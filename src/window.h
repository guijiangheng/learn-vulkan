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

 private:
  void initWindow();

  int width;
  int height;
  std::string title;
  GLFWwindow* window;
};

}  // namespace lve
