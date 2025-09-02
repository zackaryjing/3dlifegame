cmake_minimum_required(VERSION 3.26)
project(LifeGmae)


set(CMAKE_CXX_STANDARD 23)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
endif ()


find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)


add_library(common_headers INTERFACE
        deps/stb/stb_image.h
        src/model/ModelLoader.hpp
        src/utils/Node.hpp
        src/utils/Type.hpp
        src/debug/Error.hpp
)
target_include_directories(common_headers INTERFACE ${CMAKE_SOURCE_DIR}/src)

add_library(common INTERFACE)
target_link_libraries(common INTERFACE glfw glm::glm glad::glad OpenGL::GL common_headers)

add_executable(LifeGame src/main.cpp src/shaders/Shader.cpp ./deps/stb/stb_image.h)
target_link_libraries(LifeGame PUBLIC common)

add_executable(LifeGameCubeTest
        src/tests/TestCube.cpp src/shaders/Shader.cpp
        src/model/CubeEBO.hpp)
target_link_libraries(LifeGameCubeTest PUBLIC common)

add_definitions(-DGLSL_DIR="${CMAKE_SOURCE_DIR}/src/shaders/" -DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-DTEXTURE_DIR="${CMAKE_SOURCE_DIR}/assets/texture/")
add_definitions(-DMODEL_DIR="${CMAKE_SOURCE_DIR}/assets/models/")

add_executable(LifeGame2dTest src/tests/TestLogic.cpp)
target_link_libraries(LifeGame2dTest PUBLIC common)

add_executable(LifeGameFuncTest src/tests/TestFunc.cpp)

add_executable(LifeGameNodeTest src/tests/TestNode.cpp)
target_link_libraries(LifeGameNodeTest PUBLIC common)
