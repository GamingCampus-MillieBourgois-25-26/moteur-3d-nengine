#pragma once
#include <windows.h>
#include <iostream>
#include <d3d11.h>
#include <dxgi.h>

#include <d3dcompiler.h>

#include "Window.h"
#include "AudioSystem.h"



namespace Engine {

    class Application
    {
    private:
        bool isRunning = false;
		Window window;
        AudioSystem audio;
    public:
        void Init();
        void Running();
        void Shutdown();

        void Run();

		bool getIsRunning() const { return isRunning; }
		void setIsRunning(bool running) { isRunning = running; }
    };
}