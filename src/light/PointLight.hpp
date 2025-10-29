#pragma once
#include <glad/glad.h>
#include <iostream>


#include "animation/Animation.hpp"
#include "light/Light.hpp"
#include "model/Model.hpp"
#include "shaders/Shader.hpp"

using std::cerr;
using std::endl;
using std::make_shared;
using std::shared_ptr;

class PointLight : public Light {
public:
    PointLight(const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor,
               const glm::vec3 &specularColor, const glm::vec3 &lightColor,
               const glm::vec3 &pos, float constant, float linear,
               float quadratic);

    PointLight(const glm::vec3 &color, const glm::vec3 &position,
               float constant, float linear, float quadratic);
    PointLight(const glm::vec3 &color, const glm::vec3 &position,
               unsigned int _id);

    PointLight();
    bool lightTurning = false;
    float pauseTime = static_cast<float>(glfwGetTime());
    float startTime = 0.0f;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
    unsigned int id = 0;
    Animation *lightPosAnimation;
    Animation *lightColorAnimation;
    float duration = 4.0f;
    glm::vec3 phase = {0, M_PI / 4, M_PI / 2};

    void setModel() override;

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
        lightShader.setVec3("light.color", lightColor);
    }

    void useLightUniform(const Shader &shader) override {
        shader.setVec3(std::format("pointLights[{:d}].position", id), position);
        shader.setFloat(std::format("pointLights[{:d}].constant", id),
                        attenuationConstant);
        shader.setFloat(std::format("pointLights[{:d}].linear", id),
                        attenuationLinear);
        shader.setFloat(std::format("pointLights[{:d}].quadratic", id),
                        attenuationQuadratic);
        shader.setVec3(std::format("pointLights[{:d}].ambient", id),
                       ambientColor);
        shader.setVec3(std::format("pointLights[{:d}].diffuse", id),
                       diffuseColor);
        shader.setVec3(std::format("pointLights[{:d}].specular", id),
                       specularColor);
    }


    void drawLight(glm::mat4 view, glm::mat4 projection) override {
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

inline PointLight::PointLight(const glm::vec3 &ambientColor,
                              const glm::vec3 &diffuseColor,
                              const glm::vec3 &specularColor,
                              const glm::vec3 &lightColor, const glm::vec3 &pos,
                              float constant, float linear, float quadratic) :
    Light(ambientColor, diffuseColor, specularColor, lightColor, pos),
    attenuationConstant(constant), attenuationLinear(linear),
    attenuationQuadratic(quadratic) {
    auto curTime = static_cast<float>(glfwGetTime());
    lightPosAnimation = new RotatePosAnimation(
            duration, curTime, true, glm::vec3(0.0, 0.0, 1.0), 360,
            lightModel->modelMat, this->position);
    lightColorAnimation =
            new Vec3SinAnimation(duration * 4, curTime, true, glm::vec3(0.5),
                                 {duration / 2, duration * 2, duration},
                                 glm::vec3(0.75), phase, this->lightColor);
}


inline PointLight::PointLight(const glm::vec3 &color, const glm::vec3 &position,
                              float constant, float linear, float quadratic) :
    PointLight(color, color, color, color, position, constant, linear,
               quadratic) {}


inline PointLight::PointLight(const glm::vec3 &color, const glm::vec3 &position,
                              unsigned int _id) :
    PointLight(color, color, color, color, position, 1.0f, 0.09f, 0.032) {
    id = _id;
}


inline PointLight::PointLight() :
    PointLight(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f,
               0.09f, 0.032f) {}

inline void PointLight::setModel() {
    lightModel = make_shared<Model>(Model::getCube());
    lightModel->modelMat = glm::translate(lightModel->modelMat, position);
}
