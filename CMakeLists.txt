cmake_minimum_required(VERSION 3.26)
project(LifeGmae)


set(CMAKE_CXX_STANDARD 23)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(LifeGame src/main.cpp src/shaders/shader.cpp ./deps/stb/stb_image.h)
target_link_libraries(LifeGame PRIVATE glfw glm::glm opengl32 glad::glad)

set(GLSL_FILES
        src/shaders/shader.fs
        src/shaders/shader.vs
)

add_definitions(-DGLSL_DIR="${CMAKE_SOURCE_DIR}/src/shaders/")
add_definitions(-DTEXTURE_DIR="${CMAKE_SOURCE_DIR}/assets/texture/")


add_executable(LifeGame2dTest src/tests/testLogic.cpp)
