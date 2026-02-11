#include "Engine/Application.h"

int main()
{
    Engine::Application app;

    // Initialize window + renderer
    app.Init();



    // Shutdown handled inside Run()
    return 0;
}
