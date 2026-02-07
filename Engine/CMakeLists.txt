# --------------------------
# Engine library
# --------------------------
add_library(Engine STATIC)

# --------------------------
# Sources
# --------------------------
target_sources(Engine
    PRIVATE
        src/Application.cpp
        src/Window.cpp
)
# --------------------------
# Include directories
# --------------------------
target_include_directories(Engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    	${CMAKE_CURRENT_SOURCE_DIR}/src
)

# --------------------------
# Définitions pour Windows
# --------------------------
target_compile_definitions(Engine
    PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)

# --------------------------
# C++ standard
# --------------------------
target_compile_features(Engine PUBLIC cxx_std_20)

# --------------------------
# Options de compilation MSVC
# --------------------------
if(MSVC)
    target_compile_options(Engine PRIVATE /W4 /permissive-)
endif()

# --------------------------
# Liens vers Windows libs
# --------------------------
target_link_libraries(Engine
    PRIVATE
        user32   # pour MessageBox et Win32
        kernel32
	glfw
    	d3d11
        dxgi
    	d3dcompiler
)
