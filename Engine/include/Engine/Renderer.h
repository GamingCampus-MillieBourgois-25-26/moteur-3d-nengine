#pragma once

#include <d3d11.h>
#include <dxgi.h>

struct GLFWwindow;

class Renderer
{
public:
    bool Initialize(GLFWwindow* window);
    void Render();
    void Shutdown();

public:
    ID3D11Device* GetDevice() const;

private:
    void CreateRenderTarget();
    void CreateShaders();
    void CreateVertexBuffer();

private:
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;
    IDXGISwapChain* m_swapChain = nullptr;
    ID3D11RenderTargetView* m_renderTargetView = nullptr;

    ID3D11VertexShader* m_vertexShader = nullptr;
    ID3D11PixelShader* m_pixelShader = nullptr;
    ID3D11InputLayout* m_inputLayout = nullptr;
    ID3D11Buffer* m_vertexBuffer = nullptr;
};
