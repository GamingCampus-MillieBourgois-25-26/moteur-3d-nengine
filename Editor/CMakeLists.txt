add_executable(Editor
    src/main.cpp
)

target_include_directories(Editor
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Engine/include
)

target_link_libraries(Editor
    PRIVATE Engine
            ${CMAKE_SOURCE_DIR}/Engine/Assets/FMOD/api/core/lib/x64/fmod_vc.lib
            ${CMAKE_SOURCE_DIR}/Engine/Assets/FMOD/api/studio/lib/x64/fmodstudio_vc.lib
)

set_target_properties(Editor PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Editor>
)
