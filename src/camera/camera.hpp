#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

struct Camera {
    static inline auto cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    static inline auto cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    static inline auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    static inline auto direction = glm::vec3(0.0f, 0.0f, -1.0f);
    static inline float cameraSpeed = 2.5;
    static inline float yaw = -90.0f;
    static inline float pitch = 0.0f;
    static inline float fov = 45.0f;


    static auto getView() {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    static auto getProjection() {
        return glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f,
                                100.0f);
    }


    static void setFov(const float _fov) { fov = _fov; }

    static void lookAround(const float yoffset, const float xoffset) {
        pitch += yoffset;
        if (pitch > 89.0f)
            pitch = 89.0f;

        if (pitch < -89.0f)
            pitch = -89.0f;
        yaw += xoffset;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        cameraFront = direction;
    }

    static void moveForward(const float deltaTime) {
        cameraPos += deltaTime * cameraSpeed * cameraFront;
    }

    static void moveBackward(const float deltaTime) {
        cameraPos -= deltaTime * cameraSpeed * cameraFront;
    }

    static void moveRight(const float deltaTime) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
                     cameraSpeed * deltaTime;
    }
    static void moveLeft(const float deltaTime) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
                     cameraSpeed * deltaTime;
    }
};
