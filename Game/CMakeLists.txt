# Build the game module as a DLL and the launcher exe

# GameModule (DLL with gameplay code)
add_library(GameModule SHARED
    "src/GameModule.cpp"
    "src/PlayerController.cpp"

)

target_include_directories(GameModule
    PRIVATE
        ${CMAKE_SOURCE_DIR}/Engine/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Ensure DLL is emitted into the Game runtime folder
set_target_properties(GameModule PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/Game
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Game
    COMMENT "GameModule runtime directory"
)