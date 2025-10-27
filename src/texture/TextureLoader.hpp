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

inline unsigned int loadTexture(const string &path) {
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

inline unsigned int loadCubeMap(vector<string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); ++i) {
        unsigned char *data =
                stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
                         height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            cerr << "Cubemap failed to load at path: " << faces[i] << endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}

inline unsigned int createBrickWallTexture() {
    return loadTexture(TEXTURE_DIR "brick_wall.jpg");
}

inline unsigned int createBoxDiffuseTexture() {
    return loadTexture(TEXTURE_DIR "wooden_box_diffuse.png");
}

inline unsigned int createBoxSpecularTexture() {
    return loadTexture(TEXTURE_DIR "wooden_box_specular.png");
}
