cmake_minimum_required(VERSION 3.20)
project(moteur_3d_nengine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(_WINDOWS)

# Sous-projets
add_subdirectory(Engine)
add_subdirectory(Game)
add_subdirectory(Editor)
