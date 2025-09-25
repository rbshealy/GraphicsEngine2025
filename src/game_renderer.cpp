//
// Created by raysh on 9/24/2025.
//

#include "game_renderer.h"

#include <array>
#include <stdexcept>


namespace GEngine{

    GameRenderer::GameRenderer(GameWindow &window, GameDevice &device) : gameWindow(window), gameDevice(device) {
        recreateSwapChain();
        createCommandBuffers();
    }

    GameRenderer::~GameRenderer() {
        freeCommandBuffers();
    }

    void GameRenderer::recreateSwapChain() {
        auto extent = gameWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = gameWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(gameDevice.device());
        gameSwapChain = nullptr;

        if (gameSwapChain == nullptr) {
            gameSwapChain = std::make_unique<class GameSwapChain>(gameDevice, extent);
    } else {
        std::shared_ptr<GameSwapChain> oldSwapChain = std::move(gameSwapChain);
        gameSwapChain = std::make_unique<GameSwapChain>(gameDevice, extent, oldSwapChain);

        if (!oldSwapChain->compareSwapFormats(*gameSwapChain.get())) {
            throw std::runtime_error("Swap Chain Image (or depth) format has changed");
        }

    }

        // add function call later
    }

    void GameRenderer::createCommandBuffers() {
        commandBuffers.resize(GameSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = gameDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(gameDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers");
        }
    }

    VkCommandBuffer GameRenderer::beginFrame() {
        assert(!isFrameStarted && "Can't call begin frame while already in progress");

        auto result = gameSwapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {   // out of date (not out of memory!)
            recreateSwapChain();
            return nullptr;
        }

        if (result == VK_ERROR_OUT_OF_DEVICE_MEMORY) {
            recreateSwapChain();
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire next image");
        }

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer");
        }

        return commandBuffer;
    }
    void GameRenderer::endFrame() {
        assert(isFrameStarted && "can't call end frame while frame is not in progress");
        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer");
        }
        auto result = gameSwapChain->submitCommandBuffers(&commandBuffer,&currentImageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || gameWindow.wasWindowResized()) {
            gameWindow.resetWindowResizeFlag();
            recreateSwapChain();
            isFrameStarted = false;
            currentFrameIndex = (currentFrameIndex + 1) % GameSwapChain::MAX_FRAMES_IN_FLIGHT;
            return;
        }
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % GameSwapChain::MAX_FRAMES_IN_FLIGHT;
    }
    void GameRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = gameSwapChain->getRenderPass();
        renderPassInfo.framebuffer = gameSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = gameSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(gameSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(gameSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor = {{0,0},gameSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }
    void GameRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

        vkCmdEndRenderPass(commandBuffer);
    }

    void GameRenderer::freeCommandBuffers() {
        vkFreeCommandBuffers(
            gameDevice.device(),
            gameDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

}
