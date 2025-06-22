#pragma once
#include <iostream>

inline auto vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "out vec4 vertexColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos,1.0);\n"
        "   vertexColor = vec4(0.5,0.0,0.0,1.0);\n"
        "}\0";

inline auto fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec4 vertexColor;\n"
        "uniform vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = ourColor;\n"
        "}\n";

inline unsigned int genVertexShader() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    int vertexCompileSuccess;
    char infolog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &vertexCompileSuccess);
    if (not vertexCompileSuccess) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infolog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
    } else {
        std::cout << "INFO::SHADER::VERTEX::COMPILATION_SUCCESS" << std::endl;
    }
    return vertexShader;
}

inline unsigned int genFragmentShader() {
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    int fragmentCompileSuccess;
    char infolog[512];
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &fragmentCompileSuccess);
    if (not fragmentCompileSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infolog);
        std::cerr << "ERROR::SHADER::fragment::COMPILATION_FAILED\n" << infolog << std::endl;
    } else {
        std::cout << "INFO::SHADER::fragment::COMPILATION_SUCCESS" << std::endl;
    }
    return fragmentShader;
}

inline unsigned int genShaderProgram() {
    const unsigned int vertexShader = genVertexShader();
    const unsigned int fragmentShader = genFragmentShader();
    const unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}
