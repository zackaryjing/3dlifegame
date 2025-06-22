cmake_minimum_required(VERSION 3.20)
project(LifeGmae)


set(CMAKE_CXX_STANDARD 23)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(LifeGame src/main.cpp src/shaders/shader.h)
target_link_libraries(LifeGame PRIVATE glfw glm::glm opengl32 glad::glad)



