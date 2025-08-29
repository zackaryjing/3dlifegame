cmake_minimum_required(VERSION 3.26)
project(LifeGmae)


set(CMAKE_CXX_STANDARD 23)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_library(common INTERFACE)

target_link_libraries(common INTERFACE glfw glm::glm glad::glad OpenGL::GL)

add_executable(LifeGame src/main.cpp src/shaders/shader.cpp ./deps/stb/stb_image.h)
target_link_libraries(LifeGame PUBLIC common)
target_include_directories(LifeGame PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(LifeGameCubeTest src/tests/TestCube.cpp src/shaders/shader.cpp ./deps/stb/stb_image.h)
target_link_libraries(LifeGameCubeTest PUBLIC common)
target_include_directories(LifeGameCubeTest PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_definitions(-DGLSL_DIR="${CMAKE_SOURCE_DIR}/src/shaders/" -DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-DTEXTURE_DIR="${CMAKE_SOURCE_DIR}/assets/texture/")

add_executable(LifeGame2dTest src/tests/TestLogic.cpp)
target_include_directories(LifeGame2dTest PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(LifeGame2dTest PUBLIC common)
