#include "Engine/Renderer.h"

#define GLFW_EXPOSE_NATIVE_WIN32

#include <Windows.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

struct Vertex
{
    float x;
    float y;
    float z;
};

bool Renderer::Initialize(GLFWwindow* window)
{
    HWND hwnd = glfwGetWin32Window(window);

    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &m_swapChain,
        &m_device,
        nullptr,
        &m_context
    );

    if (FAILED(result))
        return false;

    CreateRenderTarget();
    CreateShaders();
    CreateVertexBuffer();

    D3D11_VIEWPORT viewport{};
    viewport.Width = 800.0f;
    viewport.Height = 600.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_context->RSSetViewports(1, &viewport);

    return true;
}

void Renderer::CreateRenderTarget()
{
    ID3D11Texture2D* backBuffer = nullptr;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

    m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
    backBuffer->Release();

    m_context->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
}

void Renderer::CreateShaders()
{
    const char* vsSrc =
        "struct VSInput { float3 position : POSITION; };"
        "struct VSOutput { float4 position : SV_POSITION; };"
        "VSOutput main(VSInput input) {"
        "    VSOutput o;"
        "    o.position = float4(input.position, 1.0f);"
        "    return o;"
        "}";

    const char* psSrc =
        "float4 main() : SV_Target {"
        "    return float4(1, 0, 0, 1);"
        "}";

    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;

    D3DCompile(vsSrc, strlen(vsSrc), nullptr, nullptr, nullptr,
        "main", "vs_5_0", 0, 0, &vsBlob, nullptr);

    D3DCompile(psSrc, strlen(psSrc), nullptr, nullptr, nullptr,
        "main", "ps_5_0", 0, 0, &psBlob, nullptr);

    m_device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &m_vertexShader
    );

    m_device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &m_pixelShader
    );

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
          D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    m_device->CreateInputLayout(
        layoutDesc,
        1,
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &m_inputLayout
    );

    vsBlob->Release();
    psBlob->Release();
}

void Renderer::CreateVertexBuffer()
{
    Vertex vertices[] =
    {
        {  0.0f,  0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };

    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA data{};
    data.pSysMem = vertices;

    m_device->CreateBuffer(&bd, &data, &m_vertexBuffer);
}

void Renderer::Render()
{
    float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    m_context->ClearRenderTargetView(m_renderTargetView, clearColor);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    m_context->IASetInputLayout(m_inputLayout);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_context->VSSetShader(m_vertexShader, nullptr, 0);
    m_context->PSSetShader(m_pixelShader, nullptr, 0);

    m_context->Draw(3, 0);

    m_swapChain->Present(1, 0);
}

void Renderer::Shutdown()
{
    if (m_vertexBuffer) m_vertexBuffer->Release();
    if (m_inputLayout) m_inputLayout->Release();
    if (m_vertexShader) m_vertexShader->Release();
    if (m_pixelShader) m_pixelShader->Release();
    if (m_renderTargetView) m_renderTargetView->Release();
    if (m_swapChain) m_swapChain->Release();
    if (m_context) m_context->Release();
    if (m_device) m_device->Release();
}

ID3D11Device* Renderer::GetDevice() const
{
    return m_device;
}
