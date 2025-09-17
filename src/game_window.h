//
// Created by raysh on 9/16/2025.
//


#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace GEngine {

    class GameWindow {

      public:
        GameWindow(int w, int h, std::string name);
        ~GameWindow();

        // delete copy and address for this class
        GameWindow(const GameWindow&) = delete;
        GameWindow &operator=(const GameWindow&) = delete;

        bool shouldClose() {return glfwWindowShouldClose(window);}
      private:

        void initWindow();

        const int width;
        const int height;

        std::string windowName;
        GLFWwindow *window;

      };
}