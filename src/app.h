#pragma once

#include "pipeline.h"
#include "window.h"

namespace lve {

class App {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();

 private:
  Window window{WIDTH, HEIGHT, "Hello Vulkan!"};
  Device device{window};
  Pipeline pipeline{device,
                    "src/shaders/simple_shader.vert.spv",
                    "src/shaders/simple_shader.frag.spv",
                    Pipeline::makeDefaultPipelineConfig(WIDTH, HEIGHT)};
};

}  // namespace lve
