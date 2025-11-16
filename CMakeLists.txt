cmake_minimum_required(VERSION 3.19.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(falling_sand_simulation)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)


add_executable(falling_sand_simulation
    src/main.cpp
    src/grid.cpp
    src/simulation.cpp
    src/external/glad.c
)

target_include_directories(falling_sand_simulation PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include      # for header files
    ${CMAKE_CURRENT_SOURCE_DIR}/libs         # for external header files
)

target_link_libraries(falling_sand_simulation PRIVATE OpenGL::GL glfw)
