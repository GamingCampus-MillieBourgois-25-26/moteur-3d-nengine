#pragma once
#include <windows.h>
#include <iostream>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>


namespace Engine {

    class Application
    {
    private:
        bool isRunning = false;

    public:
        void Init();
        void Running();
        void Shutdown();

        void Run();
    };
}