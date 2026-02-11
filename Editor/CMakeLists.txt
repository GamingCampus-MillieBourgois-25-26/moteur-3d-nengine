add_executable(Editor
    src/main.cpp
)

# Link Engine
target_link_libraries(Editor
    PRIVATE Engine
)

# Debugger working directory = dossier de l'exe
set_target_properties(Editor PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Editor>
)