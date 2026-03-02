cmake_minimum_required(VERSION 3.20)
project(moteur_3d_nengine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.9
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)
FetchContent_Declare(
  bullet3
  GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
  GIT_TAG        3.25
)

set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(INSTALL_LIBS OFF CACHE BOOL "" FORCE)
set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(bullet3)

add_subdirectory(Engine)
add_subdirectory(Game)
add_subdirectory(Editor)



