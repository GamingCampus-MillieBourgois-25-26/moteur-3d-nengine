#pragma once
#include <windows.h>
#include <iostream>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

#include "Window.h"
#include "AudioSystem.h"
#include "Renderer.h"

#include "Input.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/MovementSystem.h"
#include <chrono>
#include <filesystem>
#include <string>
#include <vector>

namespace Engine {

    class Application
    {
    private:
        bool isRunning = false;

        Renderer renderer;
        AudioSystem audio;
        WindowInstance window;
        std::unique_ptr<Input> input;
        Coordinator coord;
        std::shared_ptr<RenderSystem> renderSystem;
        std::shared_ptr<MovementSystem> movementSystem;

        using clock = std::chrono::high_resolution_clock;

		float mouseSensitivity = 0.002f;
		float speed = 0.f;

    public:

        void Init();
        void Running();
        void Shutdown();

        bool getIsRunning() const { return isRunning; }
        void setIsRunning(bool running) { isRunning = running; }
    };
}
