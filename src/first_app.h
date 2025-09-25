//
// Created by raysh on 9/16/2025.
//

#pragma once

#include <game_window.h>
#include <game_device.h>
#include <game_object.h>
#include <game_renderer.h>

#include <memory>
#include <vector>

namespace GEngine {

    class FirstApp{

      public:
      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      FirstApp();
      ~FirstApp();


      FirstApp(const FirstApp&) = delete;
      FirstApp& operator=(const FirstApp&) = delete;

        void run();

      private:
        void loadGameObjects();

        GameWindow gameWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
        GameDevice gameDevice{gameWindow};
        GameRenderer gameRenderer{gameWindow, gameDevice};

        std::vector<GameObject> gameObjects;
      };
}

