//
// Created by raysh on 9/16/2025.
//

#include <game_window.h>

#include <stdexcept>

namespace GEngine{

    GameWindow::GameWindow(int w, int h, std::string name) : width(w), height(h), windowName(name) {
      initWindow();
      }

    GameWindow::~GameWindow() {
      glfwDestroyWindow(window);
      glfwTerminate();
    }

    void GameWindow::initWindow() {
      glfwInit();
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

      window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
      }

  void GameWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void GameWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto GameWindow = reinterpret_cast<class GameWindow *>(glfwGetWindowUserPointer(window));
        GameWindow->frameBufferResized = true;
        GameWindow->width = width;
        GameWindow->height = height;
    }


}