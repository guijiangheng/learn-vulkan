#include "window.h"

namespace lve {

Window::Window(int width, int height, std::string title)
    : width{width}, height{height}, title{title} {
  initWindow();
}

Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

bool Window::shouldClose() const { return glfwWindowShouldClose(window); }

}  // namespace lve
