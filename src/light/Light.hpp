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
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    glm::vec3 lightColor;
    glm::vec3 position;
    string lighting_fshader = GLSL_DIR "LightingShader.fs";
    string lighting_vshader = GLSL_DIR "LightingShader.vs";
    Shader lightShader = {};
    shared_ptr<Model> lightModel;
    const bool lightTurning = true;

    void propertySpin();
    void setCommonUni(glm::mat4 view, glm::mat4 projection) {
        lightShader.setInt("ourTexture", 0);
        lightShader.setMatrix4("view", view);
        lightShader.setMatrix4("projection", projection);
    }

    void setModelUni() {
        lightShader.setVec3("light.position", position);
        lightShader.setVec3("light.ambient", ambientColor);
        lightShader.setVec3("light.diffuse", diffuseColor);
        lightShader.setVec3("light.specular", specularColor);
        lightShader.setVec3("light.color", lightColor);
    }

    void drawLight(glm::mat4 view, glm::mat4 projection) {
        if (lightTurning) {
            propertySpin();
        }
        lightShader.use();
        setCommonUni(view, projection);
        setModelUni();
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        lightShader.setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, lightModel->dataPos.first,
                     lightModel->dataPos.second);
    }
};

Light::Light() {
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

void Light::propertySpin() {

    lightColor.x = (float) sin(glfwGetTime() * 2.0f) * 0.5f + 1;
    lightColor.y = (float) sin(glfwGetTime() * 0.7f) * 0.5f + 1;
    lightColor.y = (float) sin(glfwGetTime() * 1.3f) * 0.5f + 1;

    position = glm::vec3(
            glm::rotate(glm::mat4(1.0f),
                        static_cast<float>(glfwGetTime()) * glm::radians(50.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::vec4(0.0f, 3.0f, 0.0f, 0.0f));

    diffuseColor = lightColor * glm::vec3(0.5f);
    ambientColor = lightColor * glm::vec3(0.2f);
}
