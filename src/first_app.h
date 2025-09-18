//
// Created by raysh on 9/16/2025.
//

#pragma once

#include <game_window.h>
#include <game_pipeline.h>

namespace GEngine {

    class FirstApp{

      public:
      static constexpr int WIDTH = 800;
      static constexpr int HEIGHT = 600;

      void run();
      private:
        GameWindow GameWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
        GamePipeline GamePipeline{"shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};

      };
}

