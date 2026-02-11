# Engine library
add_library(Engine STATIC)

# Sources et headers
target_sources(Engine
    PRIVATE
        src/Application.cpp
        src/Renderer.cpp
        src/Window.cpp
	    src/AudioSystem.cpp
        src/OBJLoader.cpp
)

target_include_directories(Engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    	${CMAKE_CURRENT_SOURCE_DIR}/src
    	${CMAKE_CURRENT_SOURCE_DIR}/NotToIgnore/FMOD/api/core/inc
    	${CMAKE_CURRENT_SOURCE_DIR}/NotToIgnore/FMOD/api/studio/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Audio
        ${CMAKE_CURRENT_SOURCE_DIR}/Assets
)

# Windows definitions
target_compile_definitions(Engine
    PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)

# C++20
target_compile_features(Engine PUBLIC cxx_std_20)

# Compiler options MSVC
if(MSVC)
    target_compile_options(Engine PRIVATE /W4 /permissive-)
endif()

# Link libraries
target_link_libraries(Engine
    PRIVATE
        d3d11
        dxgi
        d3dcompiler
        glfw
        opengl32
)