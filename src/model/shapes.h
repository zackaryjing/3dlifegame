
struct Cube {
    static inline float vertices[] = {
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
            -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, //
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
            0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, //
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
    };
    static inline int indices[] = {
            0, 1, 2, //
            2, 3, 0, //
            6, 5, 4, //
            4, 7, 6, //

            7, 4, 0, //
            0, 3, 7, //
            1, 5, 6, //
            6, 2, 1, //

            4, 5, 1, //
            1, 0, 4, //
            7, 3, 2, //
            2, 6, 7, //
    };

    // typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint
    // size, GLenum type, GLboolean normalized, GLsizei stride, const void
    // *pointer);
    static inline GLuint vertices_index = 0;
    static inline GLint vertices_size = 3;
    static inline GLsizei vertices_stride = 5 * sizeof(float);
    static inline auto vertices_pointer =
            reinterpret_cast<void *>(0 * sizeof(float));
    // texture coordinate
    static inline GLuint coord_index = 1;
    static inline GLint coord_size = 2;
    static inline GLsizei coord_stride = 5 * sizeof(float);
    static inline auto coord_pointer =
            reinterpret_cast<void *>(3 * sizeof(float));
    static inline GLsizei vertices_cnt = 8;
    static inline GLsizei indices_cnt = 36;
};

// -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
// 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
// -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, //
// -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
// 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, //
// 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
// -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
