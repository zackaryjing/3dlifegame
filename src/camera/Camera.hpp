#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

#include "ui/Window.hpp"

struct Camera {
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float cameraSpeed = 2.5;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 45.0f;


    auto getView() {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    auto getProjection() {
        return glm::perspective(glm::radians(fov),
                                (float) Window::width / (float) Window::height,
                                0.1f, 100.0f);
    }


    void setFov(const float _fov) { fov = _fov; }

    void lookAround(const float yoffset, const float xoffset) {
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
        cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
        cameraUp = glm::cross(cameraRight, cameraFront);
    }

    void moveForward(const float deltaTime) {
        cameraPos += deltaTime * cameraSpeed * cameraFront;
    }

    void moveBackward(const float deltaTime) {
        cameraPos -= deltaTime * cameraSpeed * cameraFront;
    }

    void moveUpward(const float deltaTime) {
        cameraPos += deltaTime * cameraSpeed * cameraUp;
    }

    void moveDownward(const float deltaTime) {
        cameraPos -= deltaTime * cameraSpeed * cameraUp;
    }

    void moveRight(const float deltaTime) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
                     cameraSpeed * deltaTime;
    }
    void moveLeft(const float deltaTime) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
                     cameraSpeed * deltaTime;
    }
};
