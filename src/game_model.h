//
// Created by raysh on 9/21/2025.
//

#pragma once

#include <game_device.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace GEngine {

class GameModel {
  public:
    struct Vertex{
      glm::vec3 position;
      glm::vec3 color;

      static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
      static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
      };

  GameModel(GameDevice &device, const std::vector<Vertex> &vertices);
  ~GameModel();

  GameModel(const GameModel&) = delete;
  GameModel& operator=(const GameModel&) = delete;

 void bind(VkCommandBuffer commandBuffer);
 void draw(VkCommandBuffer commandBuffer);

  private:
    GameDevice& GameDevice;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;

    void createVertexBuffer(const std::vector<Vertex>& vertices);

};

} // GEngine

