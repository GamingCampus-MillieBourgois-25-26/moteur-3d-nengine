#pragma once
#include <windows.h>
#include <iostream>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

#include "Window.h"
#include "AudioSystem.h"
#include "OBJLoader.h"
#include "Renderer.h"
#include "ScriptManager.h"
#include "ScriptAPI.h"

#include <chrono>
#include <filesystem>
#include <string>
#include <vector>

namespace Engine {

    class Application
    {
    private:
        bool isRunning = false;

        AudioSystem audio;
        OBJLoader loader;
        WindowInstance window;

        using clock = std::chrono::high_resolution_clock;

        Renderer renderer;

        // Nouveau système de scripting
        ScriptManager scriptManager;
        std::vector<IScript*> m_scripts;

    public:

        void Init();
        void Running();
        void Shutdown();

        void Run();

        bool getIsRunning() const { return isRunning; }
        void setIsRunning(bool running) { isRunning = running; }
    };
}
