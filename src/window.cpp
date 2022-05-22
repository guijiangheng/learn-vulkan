#include "window.h"

#include <stdexcept>

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
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framerBufferResizeCallback);
}

bool Window::shouldClose() const { return glfwWindowShouldClose(window); }

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface");
  }
}

void Window::framerBufferResizeCallback(GLFWwindow* window,
                                        int width,
                                        int height) {
  auto mWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
  mWindow->framebufferResized = true;
  mWindow->width = width;
  mWindow->height = height;
}

}  // namespace lve
