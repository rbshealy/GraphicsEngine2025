//
// Created by raysh on 9/16/2025.
//

#include "first_app.h"
#include "simple_render_system.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>


namespace GEngine{

    FirstApp::FirstApp() {
        loadGameObjects();
    }

    FirstApp::~FirstApp() {
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
            SimpleRenderSystem simpleRenderSystem{gameDevice, gameRenderer.getSwapChainRenderPass()};

          while (!gameWindow.shouldClose()){
            glfwPollEvents();
                if (auto commandBuffer = gameRenderer.beginFrame()) {
                    gameRenderer.beginSwapChainRenderPass(commandBuffer);
                    simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                    gameRenderer.endSwapChainRenderPass(commandBuffer);
                    gameRenderer.endFrame();
                }
            }

        vkDeviceWaitIdle(gameDevice.device());
      }

}
