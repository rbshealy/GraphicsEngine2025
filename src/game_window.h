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
        bool wasWindowResized() {return frameBufferResized;}
        void resetWindowResizeFlag() {frameBufferResized = false;}
        VkExtent2D getExtent() {return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};};

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

      private:
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        void initWindow();

        int width;
        int height;
        bool frameBufferResized = false;

        std::string windowName;
        GLFWwindow *window;

      };
}