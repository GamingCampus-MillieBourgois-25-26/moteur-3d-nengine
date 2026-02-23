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



namespace Engine {

    class Application
    {
    private:
        bool isRunning = false;
        AudioSystem audio;
        OBJLoader loader;
        WindowInstance window;


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