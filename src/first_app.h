//
// Created by raysh on 9/16/2025.
//

#pragma once

#include <game_window.h>
#include <game_pipeline.h>
#include <game_device.h>
#include <game_object.h>
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
        void loadGameObjects();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        void renderGameObjects(VkCommandBuffer commandBuffer);

        GameWindow gameWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
        GameDevice gameDevice{gameWindow};
        std::unique_ptr<GameSwapChain> gameSwapChain;
        std::unique_ptr<GamePipeline> gamePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<GameObject> gameObjects;
      };
}

