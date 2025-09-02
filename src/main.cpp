#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <unistd.h>
#include "logic/GameOfLife.hpp"
#include "model/Cube.hpp"
#include "model/Model.hpp"
#include "model/ModelLoader.hpp"
#include "scene/Scene.hpp"
#include "ui/KeyboardInput.hpp"
#include "ui/MouseInput.hpp"
#include "ui/Window.hpp"

using namespace std;


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window::init();

    GLFWwindow *window = glfwCreateWindow(Window::width, Window::height,
                                          Window::name, nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (not gladLoadGLLoader(
                reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        cerr << "Failed to initialize GLAD" << endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    cout << "Maximum number of vertex attributes supported: " << nrAttributes
         << endl;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    glfwSetCursorPosCallback(window, Cursor::mouse_callback);
    glfwSetScrollCallback(window, Cursor::scroll_callback);
    glfwSetWindowFocusCallback(window, Cursor::window_focus_callback);

    Scene::models.emplace_back(make_shared<Model>(Model::getCube()));
    for (int i = 0; i < 5; ++i) {
        Scene::models.emplace_back(make_shared<Model>(Model::getCube()));
    }
    string model_dir = MODEL_DIR "arrow.obj";
    //    string model_dir = MODEL_DIR "cube.obj";
    Scene::models.emplace_back(
            make_shared<Model>(ModelLoader::loadModel(model_dir)));

    Scene::render(window);
    return 0;
}
