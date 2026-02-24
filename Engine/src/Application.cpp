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

    // Path to GameModule DLL produced by the Game project
    // Ajuste ce chemin selon ton build: ex. "${CMAKE_BINARY_DIR}/Game/GameModule.dll"
    m_modulePath = "GameModule.dll";

    // Try initial load
    if (m_moduleLoader.Load(m_modulePath)) {
        if (!m_moduleLoader.CallInit()) {
            std::cout << "Warning: GameModule Init returned false\n";
        }
    }
    else {
        std::cout << "Warning: initial game module failed to load. Compile the DLL later to hot-reload.\n";
    }

    isRunning = true;
}

void Engine::Application::Running()
{
    auto lastTime = clock::now();
    std::cout << "Application is running...\n";
    while (isRunning && !window.ShouldClose()) {
        auto now = clock::now();
        std::chrono::duration<float> elapsed = now - lastTime;
        float dt = elapsed.count();
        lastTime = now;

        // Update subsystems
        audio.Update();
        window.Update();

        // Check and reload the game module if it changed on disk
        m_moduleLoader.ReloadIfChanged();

        // Call module update if present
        m_moduleLoader.CallUpdate(dt);

        // Render
        renderer.Render(dt);
    }
}

void Engine::Application::Shutdown()
{
    std::cout << "Shutting down application...\n";

    // Call Shutdown and unload module
    m_moduleLoader.CallShutdown();
    m_moduleLoader.Unload();

    renderer.Shutdown();
    window.ShouldClose();
    isRunning = false;
}

void Engine::Application::Run()
{
    std::cout << "Run entry (unused)\n";
}