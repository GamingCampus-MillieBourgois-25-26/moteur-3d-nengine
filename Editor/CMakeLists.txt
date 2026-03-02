add_executable(Editor
    src/main.cpp
)



target_sources(Editor
    PRIVATE
    src/imgui.cpp
    src/imgui_draw.cpp
    src/imgui_impl_dx11.cpp
    src/imgui_impl_glfw.cpp
    src/imgui_tables.cpp
    src/imgui_widgets.cpp
)
target_include_directories(Editor
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Editor/include
        ${CMAKE_SOURCE_DIR}/Engine/include
)

target_link_libraries(Editor
    PRIVATE Engine
            ${CMAKE_SOURCE_DIR}/Engine/Assets/FMOD/api/core/lib/x64/fmod_vc.lib
            ${CMAKE_SOURCE_DIR}/Engine/Assets/FMOD/api/studio/lib/x64/fmodstudio_vc.lib
            glfw
)

set_target_properties(Editor PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Editor>
)
