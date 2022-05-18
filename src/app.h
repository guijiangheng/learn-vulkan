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
  Pipeline pipeline{"src/shaders/simple_shader.vert.spv",
                    "src/shaders/simple_shader.frag.spv"};
};

}  // namespace lve
