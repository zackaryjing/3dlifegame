#include "shader.h"
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::stringstream;

Shader::Shader(string vertexShaderParam, string fragmentShaderParam,
               ShaderParamType type) {

    string vShaderCode;
    string fShaderCode;
    if (type == ShaderParamType::PATH) {
        ifstream vShaderFile;
        ifstream fShaderFile;
        // throw an exception if failed or path is bad
        vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        try {
            vShaderFile.open(vertexShaderParam);
            fShaderFile.open(fragmentShaderParam);
            stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vShaderCode = vShaderStream.str();
            fShaderCode = fShaderStream.str();
        } catch (ifstream::failure e) {
            cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
            throw;
        }
    } else {
        vShaderCode = vertexShaderParam;
        fShaderCode = fragmentShaderParam;
    }

    char infoLog[512];

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vShaderPointer = vShaderCode.c_str();
    const char *fShaderPointer = fShaderCode.c_str();
    glShaderSource(vertexShader, 1, &vShaderPointer, nullptr);
    glCompileShader(vertexShader);
    int vertexCompileSuccess;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompileSuccess);
    if (not vertexCompileSuccess) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    } else {
        std::cout << "INFO::SHADER::VERTEX::COMPILATION_SUCCESS" << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderPointer, nullptr);
    glCompileShader(fragmentShader);
    int fragmentCompileSuccess;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompileSuccess);
    if (not fragmentCompileSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::fragment::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    } else {
        std::cout << "INFO::SHADER::fragment::COMPILATION_SUCCESS" << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    int programLinkSuccess;
    glGetProgramiv(ID, GL_LINK_STATUS, &programLinkSuccess);
    if (not programLinkSuccess) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() const { glUseProgram(ID); }

void Shader::setBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()),
                static_cast<int>(value));
}

void Shader::setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}


void Shader::setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string &name,
                      const std::initializer_list<float> &value) const {
    switch (value.size()) {
        case 1:
            glUniform1f(glGetUniformLocation(ID, name.c_str()), *value.begin());
            break;
        case 2:
            glUniform2f(glGetUniformLocation(ID, name.c_str()), *value.begin(),
                        *(value.begin() + 1));
            break;
        case 3:
            glUniform3f(glGetUniformLocation(ID, name.c_str()), *value.begin(),
                        *(value.begin() + 1), *(value.begin() + 2));
            break;
        case 4:
            glUniform4f(glGetUniformLocation(ID, name.c_str()), *value.begin(),
                        *(value.begin() + 1), *(value.begin() + 2),
                        *(value.begin() + 3));
            break;
        default:
            break;
    }
}


void Shader::setVec3(const string &name, glm::vec3 value) const {
    unsigned int transformLoc = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(transformLoc, 1, glm::value_ptr(value));
}

void Shader::setMatrix4(const string &name, glm::mat4 value) const {
    unsigned int transformLoc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}
