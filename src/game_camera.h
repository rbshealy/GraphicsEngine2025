//
// Created by raysh on 9/26/2025.
//

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#pragma once

namespace GEngine {
    class GameCamera {
    public:

        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
        void setPerspectiveProjection(float fovy, float aspect, float near, float far);

        const glm::mat4& getProjection() const {return projectionMatrix;}


    private:
        glm::mat4 projectionMatrix{1.f};
    };
} // GEngine

