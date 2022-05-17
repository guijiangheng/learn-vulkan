#include "app.h"

namespace lve {

void App::run() {
  while (!window.shouldClose()) {
    glfwPollEvents();
  }
}

}  // namespace lve
