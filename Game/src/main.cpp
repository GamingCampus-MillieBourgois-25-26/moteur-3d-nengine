#include "Engine/Application.h"

int main()
{
    Engine::Application app;

    // Initialize window + renderer
    app.Init();

    // Run main loop
    app.Running();

    // Shutdown handled inside Run()
    return 0;
}
