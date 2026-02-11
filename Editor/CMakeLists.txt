add_executable(Editor
    src/main.cpp
)

# Link Engine
target_link_libraries(Editor
    PRIVATE Engine

       ${CMAKE_SOURCE_DIR}/Engine/Assets/FMOD/api/core/lib/x64/fmod_vc.lib
       ${CMAKE_SOURCE_DIR}/Engine/Assets/FMOD/api/studio/lib/x64/fmodstudio_vc.lib
)

# Debugger working directory = dossier de l'exe
set_target_properties(Editor PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Editor>
)


