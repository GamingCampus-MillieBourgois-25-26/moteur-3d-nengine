#pragma once
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

namespace Engine {

    class Application
    {
    public:
        ID3D11Device* device = nullptr;
        ID3D11DeviceContext* context = nullptr;
        IDXGISwapChain* swapChain = nullptr;

        LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        void InitD3D(HWND hwnd);
        int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPCWSTR, int nCmdShow);

        void Run();
    };
}