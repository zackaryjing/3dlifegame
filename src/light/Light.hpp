#pragma once
#include <glad/glad.h>
#include <iostream>

using std::cerr;
using std::endl;


class Light {
    static inline bool hasInit = false;
    static inline unsigned int lightVAO;

public:
    static inline glm::vec3 ambientColor;
    static inline glm::vec3 diffuseColor;
    static inline glm::vec3 specularColor;
    static inline glm::vec3 lightColor;
    static inline glm::vec3 position;
    static inline glm::vec3 color;

    static inline void init();
    static inline unsigned int getLightVAO(unsigned int VBO);
    static inline void propertySpin();
};

inline void Light::init() {
    if (not hasInit) {
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        hasInit = true;
        ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
        diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
        specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
        position = glm::vec3(0.0f, 3.0f, 0.0f);
        color = glm::vec3(1.0f, 1.0f, 1.0f);
    }
}

inline unsigned int Light::getLightVAO(const unsigned int VBO) {
    if (not hasInit) {
        cerr << "ERROR::LIGHT::Haven't init light object" << endl;
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          reinterpret_cast<void *>(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    return lightVAO;
}

inline void Light::propertySpin() {

    lightColor.x = (float) sin(glfwGetTime() * 2.0f);
    lightColor.y = (float) sin(glfwGetTime() * 0.7f);
    lightColor.y = (float) sin(glfwGetTime() * 1.3f);

    position = glm::vec3(
            glm::rotate(glm::mat4(1.0f),
                        static_cast<float>(glfwGetTime()) * glm::radians(50.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::vec4(0.0f, 3.0f, 0.0f, 0.0f));

    diffuseColor = lightColor * glm::vec3(0.5f);
    ambientColor = lightColor * glm::vec3(0.2f);
}
