#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace lve {

class Window {
 public:
  Window(int width, int height, std::string title);
  ~Window();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  bool shouldClose() const;

  bool isWindowResized() { return framebufferResized; }

  void resetWindowResizedFlag() { framebufferResized = false; }

  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

 private:
  void initWindow();

  static void framerBufferResizeCallback(GLFWwindow* window,
                                         int width,
                                         int height);

  int width;
  int height;
  bool framebufferResized = false;

  std::string title;
  GLFWwindow* window;
};

}  // namespace lve
