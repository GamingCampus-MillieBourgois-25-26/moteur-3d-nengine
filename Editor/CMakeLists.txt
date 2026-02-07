project(Editor LANGUAGES CXX)

add_executable(Editor
    src/main.cpp
)

# Link Engine
target_link_libraries(Game
    PRIVATE Engine
)