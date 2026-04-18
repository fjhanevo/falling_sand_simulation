cmake_minimum_required(VERSION 3.19.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(falling_sand_simulation)

# --- External packages ---
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Resource path
configure_file(config.h.in config.h)

# --- Source files ---
add_executable(falling_sand_simulation
    src/main.cpp
    src/grid.cpp
    src/simulation.cpp
    src/particle_registry.cpp
    src/particles/sand.cpp
    src/particles/water.cpp
    src/particles/wood.cpp
    src/particles/fire.cpp
    src/particles/smoke.cpp
    src/external/renderer.cpp
    src/external/glad.c
)

# --- Header files ---
target_include_directories(falling_sand_simulation PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include      # for header files
    ${CMAKE_CURRENT_SOURCE_DIR}/libs         # for external header files
    ${CMAKE_BINARY_DIR}                      # make config.h visible
)

target_link_libraries(falling_sand_simulation PRIVATE OpenGL::GL glfw)
