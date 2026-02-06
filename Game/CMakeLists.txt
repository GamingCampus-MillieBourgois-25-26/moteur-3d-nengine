add_executable(Game
    src/main.cpp
)

# Link Engine
target_link_libraries(Game
    PRIVATE Engine
)

# Debugger working directory = dossier de l'exe
set_target_properties(Game PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Game>
)
