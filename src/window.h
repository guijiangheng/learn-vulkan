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

  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

 private:
  void initWindow();

  int width;
  int height;
  std::string title;
  GLFWwindow* window;
};

}  // namespace lve
