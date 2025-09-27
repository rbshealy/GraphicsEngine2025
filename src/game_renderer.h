//
// Created by raysh on 9/24/2025.
//

#pragma once

#include <game_window.h>
#include <game_device.h>
#include <game_swap_chain.h>
#include <game_model.h>

#include <memory>
#include <vector>
#include <cassert>

namespace GEngine {

    class GameRenderer{

    public:
        GameRenderer(GameWindow &window, GameDevice &device);
        ~GameRenderer();


        GameRenderer(const GameRenderer&) = delete;
        GameRenderer& operator=(const GameRenderer&) = delete;

        VkRenderPass getSwapChainRenderPass() const {return gameSwapChain->getRenderPass();}
        float getAspectRatio() const {return gameSwapChain->extentAspectRatio();}
        bool isFrameInProgress() const {return isFrameStarted;}

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        GameWindow& gameWindow;
        GameDevice& gameDevice;
        std::unique_ptr<GameSwapChain> gameSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex = 0;
        bool isFrameStarted{false};
    };
}

