#include "Engine/Application.h"
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <chrono>

using namespace std::chrono;
namespace fs = std::filesystem;

void Engine::Application::Init()
{
    std::cout << "Initializing application...\n";

    window.Create(800, 600, "My Application");

    audio.Init();
    audio.LoadBanks();
    audio.PlayEvent("event:/MSC_EFN");
    loader.loadOBJFile();

    if (!renderer.Initialize(window.GetGLFWwindow(), 800, 600))
    {
        std::cout << "Failed to initialize renderer\n";
        return;
    }

    // Charger la DLL gameplay via ScriptManager
    if (!scriptManager.LoadModule("GameModule.dll"))
    {
        std::cout << "[Application] Failed to load GameModule.dll\n";
    }
    else
    {
        // Exemple : cr�er un script PlayerController
        IScript* player = scriptManager.Create("PlayerController");
        if (player)
        {
            player->OnCreate();
            m_scripts.push_back(player);
            std::cout << "[Application] PlayerController script created\n";
        }
        else
        {
            std::cout << "[Application] Script 'PlayerController' not found in DLL\n";
        }
    }

    isRunning = true;

    Running();
}

void Engine::Application::Running()
{
    auto lastTime = clock::now();
    std::cout << "Application is running...\n";

    while (isRunning && !window.ShouldClose())
    {
        auto now = clock::now();
        std::chrono::duration<float> elapsed = now - lastTime;
        float dt = elapsed.count();
        lastTime = now;

        // Update subsystems
        audio.Update();
        window.Update();

        // Update scripts
        ScriptContext ctx{ dt };
        for (auto* script : m_scripts)
            script->OnUpdate(ctx);

        // Render
        renderer.Render(dt);
    }

    Shutdown();
}

void Engine::Application::Shutdown()
{
    std::cout << "Shutting down application...\n";

    // D�truire les scripts proprement
    for (auto* script : m_scripts)
    {
        script->OnDestroy();
        scriptManager.Destroy(script);
    }
    m_scripts.clear();

    renderer.Shutdown();
    window.ShouldClose();
    isRunning = false;
}

