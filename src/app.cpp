#include "app.h"

namespace lve {

App::App() {
  loadModels();
  createPipelineLayout();
  recreateSwapChain();
  createCommandBuffers();
}

App::~App() {
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void App::loadModels() {
  std::vector<Model::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
  };
  model = std::make_unique<Model>(device, vertices);
}

void App::createPipelineLayout() {
  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
  pipelineLayoutCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutCreateInfo.setLayoutCount = 0;
  pipelineLayoutCreateInfo.pSetLayouts = nullptr;
  pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
  pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(device.device(),
                             &pipelineLayoutCreateInfo,
                             nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void App::createPipeline() {
  PipelineConfigInfo pipelineConfig{};
  Pipeline::makeDefaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = swapchain->getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<Pipeline>(device,
                                        "src/shaders/simple_shader.vert.spv",
                                        "src/shaders/simple_shader.frag.spv",
                                        pipelineConfig);
}

void App::recreateSwapChain() {
  auto extent = window.getExtent();

  while (extent.width == 0 || extent.height == 0) {
    extent = window.getExtent();
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(device.device());

  if (swapchain == nullptr) {
    swapchain = std::make_unique<SwapChain>(device, extent);
  } else {
    swapchain =
        std::make_unique<SwapChain>(device, extent, std::move(swapchain));
    if (swapchain->imageCount() != commandBuffers.size()) {
      freeCommandBuffers();
      createCommandBuffers();
    }
  }

  createPipeline();
}

void App::createCommandBuffers() {
  commandBuffers.resize(swapchain->imageCount());
  VkCommandBufferAllocateInfo allocateInfo{};
  allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocateInfo.commandPool = device.getCommandPool();
  allocateInfo.commandBufferCount =
      static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(device.device(),
                               &allocateInfo,
                               commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers");
  }
}

void App::freeCommandBuffers() {
  vkFreeCommandBuffers(device.device(),
                       device.getCommandPool(),
                       static_cast<uint32_t>(commandBuffers.size()),
                       commandBuffers.data());
  commandBuffers.clear();
}

void App::recordCommandBuffer(int imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = swapchain->getRenderPass();
  renderPassInfo.framebuffer = swapchain->getFrameBuffer(imageIndex);
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swapchain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(
      commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapchain->getSwapChainExtent().width);
  viewport.height = static_cast<float>(swapchain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D sissor{{0, 0}, swapchain->getSwapChainExtent()};
  vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
  vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &sissor);

  pipeline->bind(commandBuffers[imageIndex]);
  model->bind(commandBuffers[imageIndex]);
  model->draw(commandBuffers[imageIndex]);

  vkCmdEndRenderPass(commandBuffers[imageIndex]);

  if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer");
  }
}

void App::drawFrame() {
  uint32_t imageIndex;
  auto result = swapchain->acquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image");
  }

  recordCommandBuffer(imageIndex);
  result =
      swapchain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      window.isWindowResized()) {
    window.resetWindowResizedFlag();
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image");
  }
}

void App::run() {
  while (!window.shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }

  vkDeviceWaitIdle(device.device());
}

}  // namespace lve
