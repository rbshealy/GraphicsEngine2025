//
// Created by raysh on 9/24/2025.
//

#pragma once

#include "game_model.h"

#include <memory>

namespace GEngine {

    struct Transform2dComponent {
        glm::vec2 translation{};
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 mat2() {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 scaleMat{{scale.x, 0.f},{0.f,scale.y}};
            glm::mat2 rotationMat{{c,s},{-s,c}};
            return rotationMat * scaleMat;
        }
    };

    class GameObject {
        public:
        using id_t = unsigned int;
        static GameObject createGameObject() {
            static id_t currentId = 0;
            return GameObject{currentId++};
        };

        GameObject(const GameObject&) = delete;
        GameObject &operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = default;
        GameObject &operator=(GameObject&&) = default;

        id_t getId() { return id; }

        std::shared_ptr<GameModel> model;
        glm::vec3 color;
        Transform2dComponent transform2d;


        private:
        explicit GameObject(id_t objId) : id{objId} {}

        id_t id;
    };
} // GEngine
