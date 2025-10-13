#pragma once
#include <vector>

#include <GLFW/glfw3.h>

using std::vector;


class Scene {
public:
    string name = "";
    unsigned VAO;
    unsigned VBO;
    vector<Group> groups;
    virtual ~Scene() = default;
    virtual void takeControl() = 0;
    Scene() = default;
    Scene(GLFWwindow *window);
    Scene(GLFWwindow *window, const string &name);
    virtual void render() = 0;
    virtual void stop() = 0;
    virtual vector<float> genGLData() const = 0;

    virtual void putDataToGL() {
        constexpr int stride = 8;
        // vertices
        constexpr GLuint vertices_index = 0;
        constexpr GLint vertices_size = 3;
        constexpr GLsizei vertices_stride = stride * sizeof(float);
        const auto vertices_pointer =
                reinterpret_cast<void *>(0 * sizeof(float));

        // texture coordinate
        constexpr GLuint coord_index = 1;
        constexpr GLint coord_size = 2;
        constexpr GLsizei coord_stride = stride * sizeof(float);
        const auto coord_pointer = reinterpret_cast<void *>(3 * sizeof(float));

        // normals
        constexpr GLuint normals_index = 2;
        constexpr GLint normals_size = 3;
        constexpr GLsizei normals_stride = stride * sizeof(float);
        const auto normals_pointer =
                reinterpret_cast<void *>(5 * sizeof(float));

        // create and bind vertex array object
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        // create vertex buffer object
        const auto vertices = genGLData();

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<long>(sizeof(float) * vertices.size()),
                     vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(vertices_index, vertices_size, GL_FLOAT, GL_FALSE,
                              vertices_stride, vertices_pointer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(coord_index, coord_size, GL_FLOAT, GL_FALSE,
                              coord_stride, coord_pointer);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(normals_index, normals_size, GL_FLOAT, GL_FALSE,
                              normals_stride, normals_pointer);
        glEnableVertexAttribArray(2);
    }
};
