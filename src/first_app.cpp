//
// Created by raysh on 9/16/2025.
//

#include "first_app.h"

#include <array>
#include <stdexcept>

namespace GEngine{

    FirstApp::FirstApp() {
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    FirstApp::~FirstApp() {
        vkDestroyPipelineLayout(GameDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::loadModels() {
        std::vector<GameModel::Vertex> vertices {
            {{0.0f,-0.5f},{1.0f,0.0f,0.0f}},
            {{0.5f, 0.5f}, {0.00f,1.0f,0.0f}},
            {{-0.5f, 0.5f}, {0.00f,0.0f,1.0f}},
        };

        GameModel = std::make_unique<class GameModel>(GameDevice,vertices);
    }


    void FirstApp::run() {
          while (!GameWindow.shouldClose()){
            glfwPollEvents();
            drawFrame();
            }

        vkDeviceWaitIdle(GameDevice.device());
      }

    void FirstApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo;
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        pipelineLayoutInfo.flags = 0;
        pipelineLayoutInfo.pNext = nullptr;
        if (vkCreatePipelineLayout(GameDevice.device(), &pipelineLayoutInfo,nullptr,&pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout");
        }
    }
    void FirstApp::createPipeline() {
        PipelineConfigInfo pipelineConfig{};
        GamePipeline::defaultPipelineConfigInfo(pipelineConfig,GameSwapChain->width(), GameSwapChain->height());
        pipelineConfig.renderPass = GameSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        GamePipeline = std::make_unique<class GamePipeline>(
            GameDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
    }

    void FirstApp::recreateSwapChain() {
        auto extent = GameWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = GameWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(GameDevice.device());
        GameSwapChain = std::make_unique<class GameSwapChain>(GameDevice, extent);
        createPipeline();
    }

    void FirstApp::createCommandBuffers() {
        commandBuffers.resize(GameSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = GameDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(GameDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers");
        }
    }

    void FirstApp::recordCommandBuffer(int imageIndex) {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = GameSwapChain->getRenderPass();
        renderPassInfo.framebuffer = GameSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = GameSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        GamePipeline->bind(commandBuffers[imageIndex]);
        GameModel->bind(commandBuffers[imageIndex]);
        GameModel->draw(commandBuffers[imageIndex]);
        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer");
        }
    }


    void FirstApp::drawFrame() {
        uint32_t imageIndex = 0;
        auto result = GameSwapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DEVICE_MEMORY) {
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire next image");
        }

        recordCommandBuffer(imageIndex);
        result = GameSwapChain->submitCommandBuffers(&commandBuffers[imageIndex],&imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || GameWindow.wasWindowResized()) {
            GameWindow.resetWindowResizeFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

}
