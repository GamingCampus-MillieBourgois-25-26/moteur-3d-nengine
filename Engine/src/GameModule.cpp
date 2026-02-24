#include <iostream>

extern "C" {

__declspec(dllexport) bool GameModule_Init()
{
    std::cout << "[GameModule] Init\n";
    // initialisation du gameplay (ressources, entités, etc.)
    return true;
}

__declspec(dllexport) void GameModule_Update(float dt)
{
    // logique de gameplay : appelée chaque frame depuis Engine::Application
    // garde l'exemple minimal:
    static float accum = 0.0f;
    accum += dt;
    if (accum >= 1.0f) {
        std::cout << "[GameModule] tick 1s\n";
        accum = 0.0f;
    }
}

__declspec(dllexport) void GameModule_Shutdown()
{
    std::cout << "[GameModule] Shutdown\n";
}

}