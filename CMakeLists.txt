cmake_minimum_required(VERSION 3.26)
project(LifeGmae)


set(CMAKE_CXX_STANDARD 23)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(LifeGame src/main.cpp src/shaders/shader.cpp ./deps/stb/stb_image.h)
target_link_libraries(LifeGame PRIVATE glfw glm::glm opengl32 glad::glad)
target_include_directories(LifeGame PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(LifeGameCubeTest src/tests/TestCube.cpp src/shaders/shader.cpp ./deps/stb/stb_image.h)
target_link_libraries(LifeGameCubeTest PRIVATE glfw glm::glm opengl32 glad::glad)
target_include_directories(LifeGameCubeTest PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_definitions(-DGLSL_DIR="${CMAKE_SOURCE_DIR}/src/shaders/" -DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-DTEXTURE_DIR="${CMAKE_SOURCE_DIR}/assets/texture/")

add_executable(LifeGame2dTest src/tests/testLogic.cpp)
target_include_directories(LifeGame2dTest PRIVATE ${CMAKE_SOURCE_DIR}/src)
