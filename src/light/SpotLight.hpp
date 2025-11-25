#pragma once
#include <glad/glad.h>
#include <iostream>


#include "animation/Animation.hpp"
#include "camera/Camera.hpp"
#include "light/Light.hpp"
#include "model/Model.hpp"
#include "model/ModelLoader.hpp"
#include "shaders/Shader.hpp"

using std::cerr;
using std::endl;
using std::make_shared;
using std::shared_ptr;

class SpotLight : public Light {
public:
    SpotLight(const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor,
              const glm::vec3 &specularColor, const glm::vec3 &lightColor,
              const glm::vec3 &pos, const glm::vec3 &dir, float cutOff,
              float outerCutOff);
    SpotLight(const glm::vec3 &color, const glm::vec3 &position,
              const glm::vec3 &direction, float cutOff, float outerCutOff);
    SpotLight();

    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
    void setModel() override;

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

    void useLightUniform(const Shader &shader) override {
        shader.setVec3("spotLight.position", position);
        shader.setVec3("spotLight.direction", direction);
        shader.setVec3("spotLight.ambient", ambientColor);
        shader.setVec3("spotLight.diffuse", diffuseColor);
        shader.setVec3("spotLight.specular", specularColor);
        shader.setFloat("spotLight.cutOff", cutOff);
        shader.setFloat("spotLight.outerCutOff", outerCutOff);
    }

    void drawLight(glm::mat4 view, glm::mat4 projection) override {
        lightShader.use();
        setCommonUniform(view, projection);
        setLightUniform();
        auto model = glm::mat4(1.0);
        model = glm::translate(model, position) * lightModel->modelMat *
                glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        lightShader.setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, lightModel->dataPos.first,
                     lightModel->dataPos.second);
    }
};

inline SpotLight::SpotLight(const glm::vec3 &ambientColor,
                            const glm::vec3 &diffuseColor,
                            const glm::vec3 &specularColor,
                            const glm::vec3 &lightColor, const glm::vec3 &pos,
                            const glm::vec3 &dir, const float cutOff,
                            const float outerCutOff) :
    Light(ambientColor, diffuseColor, specularColor, lightColor, pos),
    direction(dir), cutOff(cos(glm::radians(cutOff))),
    outerCutOff(cos(glm::radians(outerCutOff))) {
    SpotLight::setModel();
}


inline SpotLight::SpotLight(const glm::vec3 &color, const glm::vec3 &position,
                            const glm::vec3 &direction, const float cutOff,
                            const float outerCutOff) :
    SpotLight(color, color, color, color, position, direction, cutOff,
              outerCutOff) {}

inline SpotLight::SpotLight() :
    SpotLight(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f),
              glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
              glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 12.5,
              17.5) {}

inline void SpotLight::setModel() {
    string model_dir = MODEL_DIR "arrow.obj";
    const auto arrow = ModelLoader::loadModel(model_dir);
    lightModel = make_shared<Model>(arrow);
    lightModel->modelMat = glm::translate(lightModel->modelMat, position);
    constexpr auto origin = glm::vec3(0.0, 1.0, 0.0);
    const auto target = glm::normalize(direction);
    if (const float cosTheta = glm::dot(origin, target); cosTheta < -0.9999) {
        glm::vec3 axis =
                normalize(glm::cross(glm::vec3(1.0, 0.0, 0.0), origin));
        if (glm::length(axis) < 0.0001) {
            axis = normalize(glm::cross(glm::vec3(0.0, 1.0, 0.0), origin));
        }
        lightModel->modelMat =
                glm::rotate(lightModel->modelMat, glm::radians(180.0f), axis);
    } else if (cosTheta < 0.9999f) {
        const glm::vec3 axis = glm::cross(origin, target);
        const float angle = acos(glm::dot(origin, target));
        lightModel->modelMat = glm::rotate(lightModel->modelMat, angle, axis);
    }
}
