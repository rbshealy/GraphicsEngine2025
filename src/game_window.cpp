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
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

      window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
      }

  void GameWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }


}