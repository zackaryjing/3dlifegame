#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <string>

#ifndef GLSL_DIR
#define GLSL_DIR "../src/shaders/" // fallback: 编辑器静态分析用
#endif

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::stringstream;


enum class ShaderParamType { PATH, CODE };

class Shader {
public:
    unsigned int ID;
    Shader() = default;
    Shader(string vertexShaderParam, string fragmentShaderParam,
           ShaderParamType type = ShaderParamType::PATH,
           string shaderName = "");
    void use() const;
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;

    void setFloat(const string &name,
                  const std::initializer_list<float> &value) const;
    void setFloat(const string &name, float value) const;
    void setVec3(const string &name, glm::vec3 value) const;
    void setMatrix4(const string &name, glm::mat4 value) const;
};
