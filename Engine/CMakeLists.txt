# Engine library
add_library(Engine STATIC)

# Sources et headers
target_sources(Engine
    PRIVATE
        src/Application.cpp
        src/Renderer.cpp
        src/Window.cpp
	    src/AudioSystem.cpp
)

target_include_directories(Engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    	${CMAKE_CURRENT_SOURCE_DIR}/src
    	${CMAKE_CURRENT_SOURCE_DIR}/Assets/FMOD/api/core/inc
    	${CMAKE_CURRENT_SOURCE_DIR}/Assets/FMOD/api/studio/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Audio
)
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/Game/Audio"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Audio" "${CMAKE_BINARY_DIR}/Game/Audio"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/Editor/Audio"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Audio" "${CMAKE_BINARY_DIR}/Editor/Audio"
    COMMENT "Copy ${CMAKE_CURRENT_SOURCE_DIR}/Assets/Audio to ${CMAKE_BINARY_DIR}/Game/Audio"
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
