cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(cosmic_dancefloor)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE  Debug)
endif()
    
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CUDA_TOOLKIT_ROOT_DIR "/usr/local/cuda-11.1")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)

SET(SRC_DIR src/window.cpp src/mesh.cpp src/texture.cpp src/meshmodel.cpp src/camera src/shader.cpp
            src/material.cpp src/lights/light.cpp src/lights/directional_light.cpp src/lights/point_light.cpp src/lights/spot_light.cpp
            src/shadows/shadowmap.cpp src/shadows/omni_shadowmap.cpp
            src/texts/text.cpp src/skybox.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes ${FREETYPE_INCLUDE_DIRS})
add_executable(cosmic_dancefloor main.cpp)
target_sources(cosmic_dancefloor PRIVATE ${SRC_DIR})
target_link_libraries(cosmic_dancefloor GLEW::GLEW glfw assimp ${FREETYPE_LIBRARIES})
