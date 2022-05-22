#include "app.h"

namespace lve {

App::App() {
  loadModels();
  createPipelineLayout();
  createPipeline();
  createCommandBuffers();
}

App::~App() {
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void App::loadModels() {
  std::vector<Model::Vertex> vertices{
      {{0.0f, -0.5f}},
      {{0.5f, 0.5f}},
      {{-0.5f, 0.5f}},
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
  auto pipelineConfig = Pipeline::makeDefaultPipelineConfigInfo(
      swapchain.width(), swapchain.height());
  pipelineConfig.renderPass = swapchain.getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<Pipeline>(device,
                                        "src/shaders/simple_shader.vert.spv",
                                        "src/shaders/simple_shader.frag.spv",
                                        pipelineConfig);
}

void App::createCommandBuffers() {
  commandBuffers.resize(swapchain.imageCount());
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

  for (int i = 0; i < commandBuffers.size(); ++i) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapchain.getRenderPass();
    renderPassInfo.framebuffer = swapchain.getFrameBuffer(i);
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapchain.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(
        commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    pipeline->bind(commandBuffers[i]);
    model->bind(commandBuffers[i]);
    model->draw(commandBuffers[i]);

    vkCmdEndRenderPass(commandBuffers[i]);

    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer");
    }
  }
}

void App::drawFrame() {
  uint32_t imageIndex;
  auto result = swapchain.acquireNextImage(&imageIndex);

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image");
  }

  result =
      swapchain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

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
