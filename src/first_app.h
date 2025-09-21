//
// Created by raysh on 9/16/2025.
//

#pragma once

#include <game_window.h>
#include <game_pipeline.h>
#include <game_device.h>
#include <game_swap_chain.h>
#include <game_model.h>

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
        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        GameWindow GameWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
        GameDevice GameDevice{GameWindow};
        GameSwapChain GameSwapChain{GameDevice, GameWindow.getExtent()};
        std::unique_ptr<GamePipeline> GamePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<GameModel> GameModel;
      };
}

