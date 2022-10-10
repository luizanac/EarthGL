cmake_minimum_required(VERSION 3.23)
project(EarthGL)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE sources src/*.cpp src/*.hpp)
add_executable(EarthGL ${sources})

target_include_directories(EarthGL PRIVATE
        third/glm/include
        third/stb/include
        third/glfw/include
        third/glew/include
        third/fmt/include)

target_link_directories(EarthGL PRIVATE
        third/glfw/lib
        third/glew/lib/Release/x64
        third/fmt/lib)

target_link_libraries(EarthGL PRIVATE opengl32.lib glew32.lib glfw3.lib libfmtd.a)

add_custom_target(copy_deps
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/third/glew/bin/Release/x64/glew32.dll
        ${CMAKE_BINARY_DIR}/glew32.dll
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources
        ${CMAKE_CURRENT_BINARY_DIR}/Resources)
add_dependencies(EarthGL copy_deps)