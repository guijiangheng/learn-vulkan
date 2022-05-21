#include "app.h"

namespace lve {

App::App() {
  createPipelineLayout();
  createPipeline();
  createCommandBuffers();
}

App::~App() {
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void App::createPipelineLayout() {
  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
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

void App::createCommandBuffers() {}

void App::drawFrame() {}

void App::run() {
  while (!window.shouldClose()) {
    glfwPollEvents();
  }
}

}  // namespace lve
