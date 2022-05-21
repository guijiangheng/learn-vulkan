#pragma once

#include <memory>
#include <vector>

#include "pipeline.h"
#include "swapchain.h"
#include "window.h"

namespace lve {

class App {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  App();
  ~App();

  App(const App&) = delete;
  App& operator=(const App&) = delete;

  void run();

 private:
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();

  Window window{WIDTH, HEIGHT, "Hello Vulkan!"};
  Device device{window};
  SwapChain swapchain{device, window.getExtent()};
  std::unique_ptr<Pipeline> pipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
};

}  // namespace lve
