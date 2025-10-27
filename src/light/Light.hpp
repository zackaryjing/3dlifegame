#pragma once
#include <glad/glad.h>
#include <iostream>

#include "animation/Animation.hpp"
#include "camera/Camera.hpp"
#include "model/Model.hpp"
#include "shaders/Shader.hpp"

using std::cerr;
using std::endl;
using std::make_shared;
using std::shared_ptr;


class Light {
public:
    Light(const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor,
          const glm::vec3 &specularColor, const glm::vec3 &lightColor,
          const glm::vec3 &pos);
    Light(glm::vec3 color, glm::vec3 position);
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
    bool lightTurning = true;
    float pauseTime = static_cast<float>(glfwGetTime());
    float startTime = 0.0f;
    Animation *lightPosAnimation;
    Animation *lightColorAnimation;
    float duration = 4.0f;
    glm::vec3 phase = {0, M_PI / 4, M_PI / 2};

    void resetAnimationTime(const float curTime) const {
        lightPosAnimation->startTime = curTime;
        lightColorAnimation->startTime = curTime;
    }

    void propertySpin(float curTime);
    void setCommonUniform(const glm::mat4 &view,
                          const glm::mat4 &projection) const {
        lightShader.setInt("ourTexture", 0);
        lightShader.setMatrix4("view", view);
        lightShader.setMatrix4("projection", projection);
    }

    void setLightUniform() const {
        lightShader.setVec3("light.position", position);
        lightShader.setVec3("light.ambient", ambientColor);
        lightShader.setVec3("light.diffuse", diffuseColor);
        lightShader.setVec3("light.specular", specularColor);
        lightShader.setVec3("light.color", lightColor);
    }

    void drawLight(glm::mat4 view, glm::mat4 projection) {
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

inline Light::Light(const glm::vec3 &ambientColor,
                    const glm::vec3 &diffuseColor,
                    const glm::vec3 &specularColor, const glm::vec3 &lightColor,
                    const glm::vec3 &pos) :
    ambientColor(ambientColor), diffuseColor(diffuseColor),
    specularColor(specularColor), lightColor(lightColor), position(pos) {
    lightShader =
            Shader(lighting_vshader, lighting_fshader, ShaderParamType::PATH);
    lightModel = make_shared<Model>(Model::getCube());
    lightModel->modelMat = glm::translate(lightModel->modelMat, position);
    auto curTime = glfwGetTime();
    lightPosAnimation = new RotatePosAnimation(
            duration, curTime, true, glm::vec3(0.0, 0.0, 1.0), 360,
            lightModel->modelMat, this->position);
    lightColorAnimation =
            new Vec3SinAnimation(duration * 4, curTime, true, glm::vec3(0.5),
                                 {duration / 2, duration * 2, duration},
                                 glm::vec3(0.75), phase, this->lightColor);
}


inline Light::Light(const glm::vec3 color, const glm::vec3 position) :
    Light(color, color, color, color, position) {}

inline Light::Light() :
    Light(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f),
          glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
          glm::vec3(0.0f, 3.0f, 0.0f)) {}
