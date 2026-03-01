#pragma once
#include <windows.h>
#include <iostream>
#include <d3d11.h>
#include <dxgi.h>

#include <d3dcompiler.h>

#include "Window.h"

#include "AudioSystem.h"
#include "OBJ/OBJLoader.h"
#include "Renderer.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/MovementSystem.h"
#include <chrono>

namespace Engine {

    class Application
    {
    private:
        bool isRunning = false;

        AudioSystem audio;
        OBJLoader loader;
        WindowInstance window;

        // ECS 

        Coordinator coord;
        std::shared_ptr<RenderSystem> renderSystem;
        std::shared_ptr<MovementSystem> movementSystem;

        using clock = std::chrono::high_resolution_clock;



        Renderer renderer;
    public:

        void Init();
        void Running();
        void Shutdown();

        void Run();

        bool getIsRunning() const { return isRunning; }
        void setIsRunning(bool running) { isRunning = running; }
    };
}