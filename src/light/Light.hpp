#pragma once
#include <glad/glad.h>
#include <iostream>
#include "camera/Camera.hpp"
#include "model/Model.hpp"
#include "shaders/Shader.hpp"

using std::cerr;
using std::endl;
using std::make_shared;
using std::shared_ptr;


class Light {
    unsigned int lightVAO;

public:
    Light();
    Light(glm::vec3 color, glm::vec3 position);
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    glm::vec3 lightColor;
    glm::vec3 position;
    string lighting_fshader = GLSL_DIR "LightingShader.fs";
    string lighting_vshader = GLSL_DIR "LightingShader.vs";
    Shader lightShader = {};
    shared_ptr<Model> lightModel;
    bool lightTurning = false;
    float pauseTime = static_cast<float>(glfwGetTime());
    float startTime = 0.0f;

    void resetTime() {
        if (not lightTurning) {
            pauseTime += glfwGetTime() - startTime;
        } else {
            startTime = static_cast<float>(glfwGetTime());
        }
    }

    void propertySpin(float curTime);
    void setCommonUniform(glm::mat4 view, glm::mat4 projection) {
        lightShader.setInt("ourTexture", 0);
        lightShader.setMatrix4("view", view);
        lightShader.setMatrix4("projection", projection);
    }

    void setLightUniform() {
        lightShader.setVec3("light.position", position);
        lightShader.setVec3("light.ambient", ambientColor);
        lightShader.setVec3("light.diffuse", diffuseColor);
        lightShader.setVec3("light.specular", specularColor);
        lightShader.setVec3("light.color", lightColor);
    }

    void drawLight(glm::mat4 view, glm::mat4 projection) {
        // float curTime;
        // if (lightTurning) {
        //     curTime = static_cast<float>(glfwGetTime()) - startTime +
        //     pauseTime;
        // } else {
        //     curTime = pauseTime;
        // }
        // propertySpin(curTime);
        lightShader.use();
        setCommonUniform(view, projection);
        setLightUniform();
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        lightShader.setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, lightModel->dataPos.first,
                     lightModel->dataPos.second);
    }
};

inline Light::Light() {
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
    specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    position = glm::vec3(0.0f, 3.0f, 0.0f);
    lightShader =
            Shader(lighting_vshader, lighting_fshader, ShaderParamType::PATH);
    lightModel = make_shared<Model>(Model::getCube());
}
inline Light::Light(const glm::vec3 color, const glm::vec3 position) :
    ambientColor(color), diffuseColor(color), specularColor(color),
    lightColor(color), position(position) {
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    lightShader =
            Shader(lighting_vshader, lighting_fshader, ShaderParamType::PATH);
    lightModel = make_shared<Model>(Model::getCube());
}


inline void Light::propertySpin(float curTime) {

    lightColor.x = sin(curTime * 2.0f) * 0.25f + 0.75;
    lightColor.y = sin(curTime * 0.7f) * 0.25f + 0.75;
    lightColor.y = sin(curTime * 1.3f) * 0.25f + 0.75;

    position = glm::vec3(glm::rotate(glm::mat4(1.0f),
                                     curTime * glm::radians(50.0f),
                                     glm::vec3(0.0f, 0.0f, 1.0f)) *
                         glm::vec4(0.0f, 3.0f, 0.0f, 0.0f));

    diffuseColor = lightColor * glm::vec3(0.7f);
    ambientColor = lightColor * glm::vec3(0.4f);
}
