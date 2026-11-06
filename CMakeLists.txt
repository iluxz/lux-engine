cmake_minimum_required(VERSION 3.20)
project(LuxEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# FetchContent for dependencies
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# GLAD - we'll use a pre-generated version
# For now, we'll use glad2 from github
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
)
FetchContent_MakeAvailable(glad)

# stb_image (header-only)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${stb_SOURCE_DIR})

# Engine library
file(GLOB_RECURSE ENGINE_SOURCES
    "src/*.cpp"
    "src/*.c"
)
file(GLOB_RECURSE ENGINE_HEADERS
    "include/lux/*.h"
    "include/lux/*.hpp"
)

add_library(luxengine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})
target_include_directories(luxengine PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(luxengine PUBLIC glfw glm::glm glad)

# Executable
add_executable(luxengine_app src/main.cpp)
target_link_libraries(luxengine_app PRIVATE luxengine)

# Copy assets to build directory
add_custom_command(TARGET luxengine_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:luxengine_app>/assets
)