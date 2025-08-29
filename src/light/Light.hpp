#pragma once
#include <glad/glad.h>
#include <iostream>

using std::cerr;
using std::endl;


class Light {
    static inline bool hasInit = false;
    static inline unsigned int lightVAO;

public:
    static inline void init();

    static inline unsigned int getLightVAO(unsigned int VBO);
};

inline void Light::init() {
    if (not hasInit) {
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        hasInit = true;
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
