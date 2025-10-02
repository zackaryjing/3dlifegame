cmake_minimum_required(VERSION 3.26)
if (NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "vcpkg triplet")
endif ()
project(LifeGmae)


set(CMAKE_CXX_STANDARD 26)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
endif ()


find_package(glfw3 3.3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Freetype 2.10 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_library(common_headers INTERFACE
        deps/stb/stb_image.h
        src/model/ModelLoader.hpp
        src/ui/Window.hpp
        src/utils/Node.hpp
        src/utils/Type.hpp
        src/debug/Error.hpp
        src/fonts/Font.hpp
        src/fonts/BmpLoader.hpp
        src/group/Group.hpp
        src/model/Material.hpp
        src/ui/KeyboardInput.hpp
        src/geometry/Icosahedron.hpp
        src/geometry/Sphere.hpp
        src/raster/vec3.hpp
        src/raster/ray.hpp
        src/raster/hitable.hpp
        src/raster/sphere.hpp
        src/raster/bmp.hpp
        src/raster/material.hpp
        src/raster/utils.hpp
        src/raster/aabb.hpp
        src/raster/bvh_node.hpp
)
target_link_libraries(common_headers INTERFACE Freetype::Freetype)
target_include_directories(common_headers INTERFACE ${CMAKE_SOURCE_DIR}/src)

add_library(common INTERFACE)
target_link_libraries(common INTERFACE glfw glm::glm glad::glad OpenGL::GL imgui::imgui common_headers)

add_executable(LifeGame src/main.cpp src/shaders/Shader.cpp ./deps/stb/stb_image.h)
target_link_libraries(LifeGame PUBLIC common)
target_compile_options(LifeGame PRIVATE -Werror)

add_executable(LifeGameCubeTest
        src/tests/TestCube.cpp src/shaders/Shader.cpp
        src/model/CubeEBO.hpp)
target_link_libraries(LifeGameCubeTest PUBLIC common)

add_definitions(-DGLSL_DIR="${CMAKE_SOURCE_DIR}/src/glsl/" -DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-DTEXTURE_DIR="${CMAKE_SOURCE_DIR}/assets/texture/")
add_definitions(-DMODEL_DIR="${CMAKE_SOURCE_DIR}/assets/models/")
add_definitions(-DFONT_DIR="${CMAKE_SOURCE_DIR}/assets/fonts/")

add_executable(LifeGame2dTest src/tests/TestLogic.cpp)
target_link_libraries(LifeGame2dTest PUBLIC common)

add_executable(LifeGameFuncTest src/tests/TestFunc.cpp)

add_executable(LifeGameNodeTest src/tests/TestNode.cpp)
target_link_libraries(LifeGameNodeTest PUBLIC common)

add_executable(LifeGameFontTest src/tests/TestFont.cpp)
target_link_libraries(LifeGameFontTest PUBLIC common)

add_executable(LifeGameImGuiTest src/tests/TestImGui.cpp)
target_link_libraries(LifeGameImGuiTest PUBLIC common)

find_package(OpenMP REQUIRED)
add_executable(LifeGameRasterTest src/tests/TestRaster.cpp)
target_link_libraries(LifeGameRasterTest PUBLIC common PRIVATE OpenMP::OpenMP_CXX)
target_compile_options(LifeGameRasterTest PRIVATE -fopenmp)
