#pragma once
#include <iostream>

#include "camera/Camera.hpp"
#include "model/Model.hpp"
#include "shaders/Shader.hpp"

using std::cerr;
using std::endl;
using std::make_shared;
using std::shared_ptr;

class Light {
public:
    virtual ~Light() = default;
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    glm::vec3 lightColor;
    glm::vec3 position;
    string lighting_fshader = GLSL_DIR "LightingShader.fs";
    string lighting_vshader = GLSL_DIR "LightingShader.vs";
    Shader lightShader = {};
    shared_ptr<Model> lightModel;
    virtual void setModel();
    virtual void useLightUniform(const Shader &shader) = 0;
    virtual void drawLight(glm::mat4 view, glm::mat4 projection) = 0;

    Light(const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor,
          const glm::vec3 &specularColor, const glm::vec3 &lightColor,
          const glm::vec3 &pos);
    Light(const glm::vec3 &color, const glm::vec3 &position);
    Light();
};

inline Light::Light(const glm::vec3 &ambientColor,
                    const glm::vec3 &diffuseColor,
                    const glm::vec3 &specularColor, const glm::vec3 &lightColor,
                    const glm::vec3 &pos) :
    ambientColor(ambientColor), diffuseColor(diffuseColor),
    specularColor(specularColor), lightColor(lightColor), position(pos) {
    lightShader =
            Shader(lighting_vshader, lighting_fshader, ShaderParamType::PATH);
    Light::setModel();
}


inline Light::Light(const glm::vec3 &color, const glm::vec3 &position) :
    Light(color, color, color, color, position) {}

inline Light::Light() :
    Light(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f),
          glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
          glm::vec3(0.0f, 3.0f, 0.0f)) {}

inline void Light::setModel() {
    lightModel = make_shared<Model>(Model::getCube());
    lightModel->modelMat = glm::translate(lightModel->modelMat, position);
}
