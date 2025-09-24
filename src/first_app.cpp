//
// Created by raysh on 9/16/2025.
//

#include "first_app.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>


namespace GEngine{

    struct SimplePushConstantData {
        glm::mat2 transform{1.0f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    FirstApp::FirstApp() {
        loadGameObjects();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    FirstApp::~FirstApp() {
        vkDestroyPipelineLayout(gameDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::loadGameObjects() {
        std::vector<GameModel::Vertex> vertices {
            {{-0.5f,-0.5f},{1.0f,0.0f,0.0f}},
            {{-0.5f, 0.5f}, {0.00f,1.0f,0.0f}},
            {{0.5f, 0.5f}, {0.00f,0.0f,1.0f}},
        };

        auto GameModel = std::make_shared<class GameModel>(gameDevice,vertices);

        //auto triangle = GameObject::createGameObject();
        //triangle.model = GameModel;
        //triangle.color = {0.1f, 0.8f, 0.1f};
        //triangle.transform2d.translation.x = .2f;
        //triangle.transform2d.scale = {2.f, .5f};
        //triangle.transform2d.rotation = .25f * glm::two_pi<float>();

        //gameObjects.push_back(std::move(triangle));

        //https://coolors.co/palette/461220-8c2f39-b23a48-fcb9b2-fed0bb
        std::vector<glm::vec3> colors{
          {70.f, 18.f, 32.f},
          {140.f, 47.f, 57.f},
          {178.f, 58.f, 72.f},
          {252.f, 185.f, 178.f},
          {254.f, 208.f, 187.f}  //
        };

        for (auto &col : colors) {
            col = col / 255.f;
        }

        for (auto& color : colors) {
            color = glm::pow(color, glm::vec3{2.2f});
        }
        for (int i = 0; i < 40; i++) {
            auto triangle = GameObject::createGameObject();
            triangle.model = GameModel;
            triangle.transform2d.scale = glm::vec2(.5f) + i * 0.025f;
            triangle.transform2d.rotation = i * glm::pi<float>() * .025f;
            triangle.color = colors[i % colors.size()];
            gameObjects.push_back(std::move(triangle));
        }
    }


    void FirstApp::run() {
          while (!gameWindow.shouldClose()){
            glfwPollEvents();
            drawFrame();
            }

        vkDeviceWaitIdle(gameDevice.device());
      }

    void FirstApp::createPipelineLayout() {

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);


        VkPipelineLayoutCreateInfo pipelineLayoutInfo;
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        pipelineLayoutInfo.flags = 0;
        pipelineLayoutInfo.pNext = nullptr;
        if (vkCreatePipelineLayout(gameDevice.device(), &pipelineLayoutInfo,nullptr,&pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout");
        }
    }
    void FirstApp::createPipeline() {
        PipelineConfigInfo pipelineConfig{};
        GamePipeline::defaultPipelineConfigInfo(pipelineConfig,gameSwapChain->width(), gameSwapChain->height());
        pipelineConfig.renderPass = gameSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        gamePipeline = std::make_unique<class GamePipeline>(
            gameDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
    }

    void FirstApp::recreateSwapChain() {
        auto extent = gameWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = gameWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(gameDevice.device());
        gameSwapChain = nullptr;
        gameSwapChain = std::make_unique<class GameSwapChain>(gameDevice, extent);
        createPipeline();
    }

    void FirstApp::createCommandBuffers() {
        commandBuffers.resize(gameSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = gameDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(gameDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
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
        renderPassInfo.renderPass = gameSwapChain->getRenderPass();
        renderPassInfo.framebuffer = gameSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = gameSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        renderGameObjects(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer");
        }
    }

    void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer) {
        gamePipeline->bind(commandBuffer);

        int i = 0;
        for (auto& obj : gameObjects) {
            i += 1;
            obj.transform2d.rotation =
                glm::mod<float>(obj.transform2d.rotation + 0.0003f * i, 2.f * glm::pi<float>());
        }

        for (auto& obj: gameObjects) {
            //obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.005f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();

            vkCmdPushConstants(
                commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }



    void FirstApp::drawFrame() {
        uint32_t imageIndex = 0;
        auto result = gameSwapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {   // out of date (not out of memory!)
            recreateSwapChain();
            return;
        }

        if (result == VK_ERROR_OUT_OF_DEVICE_MEMORY) {
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire next image");
        }

        vkResetCommandBuffer(commandBuffers[imageIndex], 0);
        recordCommandBuffer(imageIndex);
        result = gameSwapChain->submitCommandBuffers(&commandBuffers[imageIndex],&imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || gameWindow.wasWindowResized()) {
            gameWindow.resetWindowResizeFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

}
