#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "scene/Scene.hpp"
#include "ui/CursorInput.hpp"
#include "ui/Window.hpp"


using namespace std;

static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    Window::init();

    GLFWwindow *window = glfwCreateWindow(Window::width, Window::height,
                                          Window::name, nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    Window::updateCursorState(window);


    if (not gladLoadGLLoader(
                reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        cerr << "Failed to initialize GLAD" << endl;
        return -1;
    }

    cout << "GL version: " << glGetString(GL_VERSION) << endl;
    cout << "GL renderer: " << glGetString(GL_RENDERER) << endl;

    // order matters, have to use this after gladLoadGLLoader
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    // debug related code
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        if (glDebugMessageCallback) {
            glDebugMessageCallback(glDebugOutput, nullptr);
            // can be used to control the debug message.
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                                  nullptr, GL_TRUE);
        } else {
            cerr << "ERROR::glDebugMessageCallback not available on this driver"
                 << endl;
        }
    }

    // enable alpha blend to render text
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    cout << "Maximum number of vertex attributes supported: " << nrAttributes
         << endl;


    Scene scene(window);
    scene.render();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
