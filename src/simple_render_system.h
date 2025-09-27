//
// Created by raysh on 9/24/2025.
//


#pragma once

#include "game_camera.h"
#include <game_pipeline.h>
#include <game_device.h>
#include <game_object.h>

#include <memory>
#include <vector>

namespace GEngine {

    class SimpleRenderSystem{

    public:

        SimpleRenderSystem(GameDevice &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer,
            std::vector<GameObject> &gameObjects,
            const GameCamera &camera);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        GameDevice &gameDevice;

        std::unique_ptr<GamePipeline> gamePipeline;
        VkPipelineLayout pipelineLayout;
    };
}

