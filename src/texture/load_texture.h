#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <iostream>
#ifndef TEXTURE_DIR
#define TEXTURE_DIR "../../assets/texture/"
#endif


using std::cerr;
using std::endl;

#include "../../deps/stb/stb_image.h"

inline unsigned int create_brick_wall_texture() {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(TEXTURE_DIR "brick_wall.jpg", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cerr << "Failed to load texture" << endl;
    }

    stbi_image_free(data);
    return texture;
}
