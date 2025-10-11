#pragma once
#include <format>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>

#ifndef TEXTURE_DIR
#define TEXTURE_DIR "../../assets/texture/"
#endif


using std::cerr;
using std::cout;
using std::endl;

inline unsigned int load_texture(const string &path) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data =
            stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        GLint format;
        if (nrChannels == 4) {
            format = GL_RGBA;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else {
            format = GL_RED;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        cout << "Successfully load texture: " << path << endl;
        cout << std::format(
                        "Texture info: width {:d} , height {:d} channel {:d}",
                        width, height, nrChannels)
             << endl;
    } else {
        cerr << "Failed to load texture: " << path << endl;
    }

    stbi_image_free(data);
    return texture;
}

inline unsigned int create_brick_wall_texture() {
    return load_texture(TEXTURE_DIR "brick_wall.jpg");
}

inline unsigned int create_wooden_box_texture() {
    return load_texture(TEXTURE_DIR "wooden_box_diffuse.png");
}
