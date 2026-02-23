#include "Engine/Application.h"

int main()
{
    Engine::Application app;

    // Initialize window and renderer
    app.Init();

    // Main loop
    while (app.getIsRunning())
    {
        app.Running();  // Update + Render


    }

    // Shutdown handled by Application destructor or explicit call
    app.Shutdown();

    return 0;
}