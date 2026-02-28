#include "Engine/Renderer.h"
#include "Engine/OBJLoader.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <d3dcompiler.h>

using namespace DirectX;

bool Renderer::Initialize(GLFWwindow* window, int width, int height)
{
    m_width = width;
    m_height = height;
    m_camera.aspect = static_cast<float>(width) / static_cast<float>(height);
   

    if (!CreateDeviceAndSwapChain(window, width, height)) {
        std::cout << "ERROR: CreateDeviceAndSwapChain failed\n";
        return false;
    }

    if (!CreateRenderTargets(width, height)) {
        std::cout << "ERROR: CreateRenderTargets failed\n";
        return false;
    }

    if (!LoadShadersFromFiles(
        L"Shaders/VertexShader.hlsl",
        L"Shaders/PixelShader.hlsl"))
    {
        std::cout << "ERROR: LoadShadersFromFiles failed\n";
        return false;
    }


    if (!CreatePipelineState()) {
        std::cout << "ERROR: CreatePipelineState failed\n";
        return false;
    }

    //if (!CreateMesh()) {
    //    std::cout << "ERROR: CreateMesh failed/n";
    //    return false;
    //}

    OBJLoader loader;
    loader.setDevice(m_device);
    loader.loadOBJFile();
    loader.vertexBufferCreation();
    loader.indexBufferCreation();

    m_mesh.vertexBuffer = loader.getVertexBuffer();
    m_mesh.indexBuffer = loader.getIndexBuffer();
    m_mesh.indexCount = loader.getIndexCount();

    // Viewport
    D3D11_VIEWPORT viewport{};
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    m_context->RSSetViewports(1, &viewport);

    return true;
}

bool Renderer::CreateDeviceAndSwapChain(GLFWwindow* window, int width, int height)
{
    HWND hwnd = glfwGetWin32Window(window);

    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createFlags = 0;
    //#if defined(_DEBUG)
    //    createFlags |= D3D11_CREATE_DEVICE_DEBUG;
    //#endif

    D3D_FEATURE_LEVEL featureLevel;
    std::cout << "HWND = " << hwnd << std::endl;
    std::cout << "Width = " << width << " Height = " << height << std::endl;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &m_swapChain,
        &m_device,
        &featureLevel,
        &m_context);

    if (FAILED(hr))
    {
        std::cout << "Failed to create device and swap chain/n";

        std::cout << "D3D11CreateDeviceAndSwapChain failed, hr = 0x" << std::hex << hr << std::dec << std::endl;
        return false;
    }

    return true;
}

bool Renderer::CreateRenderTargets(int width, int height)
{
    // Back buffer → RTV
    ID3D11Texture2D* backBuffer = nullptr;
    HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (FAILED(hr)) return false;

    hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
    backBuffer->Release();
    if (FAILED(hr)) return false;

    // Depth buffer
    D3D11_TEXTURE2D_DESC depthDesc{};
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = m_device->CreateTexture2D(&depthDesc, nullptr, &m_depthStencilBuffer);
    if (FAILED(hr)) return false;

    hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, nullptr, &m_depthStencilView);
    if (FAILED(hr)) return false;

    // Bind RTV + DSV
    m_context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    return true;
}

bool Renderer::LoadShadersFromFiles(const std::wstring& vsPath, const std::wstring& psPath)
{
    std::wcout << L"Trying to load VS: " << vsPath << std::endl;
    std::wcout << L"Trying to load VS: " << psPath << std::endl;


    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    UINT compileFlags = 0;
#if defined(_DEBUG)
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr = D3DCompileFromFile(
        vsPath.c_str(), nullptr, nullptr,
        "main", "vs_5_0",
        compileFlags, 0,
        &vsBlob, &errorBlob);

    if (FAILED(hr))
    {
        std::wcout << L"VS path attempted: " << vsPath << std::endl;
        std::wcout << L"VS path attempted: " << psPath << std::endl;
        std::cout << "HRESULT = 0x" << std::hex << hr << std::dec << std::endl;

        if (errorBlob)
        {
            std::cout << "VS compile error:\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
            errorBlob->Release();
        }
        else
        {
            std::cout << "No errorBlob. Probably FILE NOT FOUND.\n";
        }

        return false;
    }


    hr = D3DCompileFromFile(
        psPath.c_str(), nullptr, nullptr,
        "main", "ps_5_0",
        compileFlags, 0,
        &psBlob, &errorBlob);

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            std::cout << "PS compile error:/n" << (char*)errorBlob->GetBufferPointer() << std::endl;
            errorBlob->Release();
        }
        vsBlob->Release();
        return false;
    }

    // Create shaders
    hr = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);
    if (FAILED(hr))
    {
        vsBlob->Release();
        psBlob->Release();
        return false;
    }

    hr = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);
    if (FAILED(hr))
    {
        vsBlob->Release();
        psBlob->Release();
        return false;
    }

    // Input layout
    D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
          D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
          D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24,
          D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = m_device->CreateInputLayout(
        layoutDesc, _countof(layoutDesc),
        vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
        &m_inputLayout);

    vsBlob->Release();
    psBlob->Release();

    if (FAILED(hr))
        return false;

    // Constant buffer
    D3D11_BUFFER_DESC cbd{};
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.ByteWidth = sizeof(ConstantBufferData);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    hr = m_device->CreateBuffer(&cbd, nullptr, &m_constantBuffer);
    if (FAILED(hr))
        return false;

    return true;
}

bool Renderer::CreatePipelineState()
{
    // Rasterizer
    D3D11_RASTERIZER_DESC rsDesc{};
    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.CullMode = D3D11_CULL_NONE;
    rsDesc.FrontCounterClockwise = FALSE;
    rsDesc.DepthClipEnable = TRUE;

    HRESULT hr = m_device->CreateRasterizerState(&rsDesc, &m_rasterizerState);
    if (FAILED(hr)) return false;

    // Depth state
    D3D11_DEPTH_STENCIL_DESC dsDesc{};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    hr = m_device->CreateDepthStencilState(&dsDesc, &m_depthState);
    if (FAILED(hr)) return false;

    return true;
}

bool Renderer::CreateMesh()
{
    // Mesh loader minimal : un simple triangle, mais via une structure Mesh
    std::vector<Vertex> vertices =
    {
        {  0.0f,  0.5f, 0.0f,   0,0,-1,   0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f,   0,0,-1,   1.0f, 1.0f },
        { -0.5f, -0.5f, 0.0f,   0,0,-1,   0.0f, 1.0f },
    };

    std::vector<UINT> indices = { 0, 1, 2 };
    m_mesh.indexCount = static_cast<UINT>(indices.size());

    // Vertex buffer
    D3D11_BUFFER_DESC vbd{};
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(Vertex));
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vdata{};
    vdata.pSysMem = vertices.data();

    HRESULT hr = m_device->CreateBuffer(&vbd, &vdata, &m_mesh.vertexBuffer);
    if (FAILED(hr)) return false;

    // Index buffer
    D3D11_BUFFER_DESC ibd{};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = static_cast<UINT>(indices.size() * sizeof(UINT));
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA idata{};
    idata.pSysMem = indices.data();

    hr = m_device->CreateBuffer(&ibd, &idata, &m_mesh.indexBuffer);
    if (FAILED(hr)) return false;

    return true;
}

void Renderer::UpdateCamera(float dt)
{
    // Pour l’instant, on fait juste tourner la caméra autour de l’origine
    static float angle = 0.0f;
    angle += dt * 0.5f;

    m_camera.position.x = sinf(angle) * 3.0f;
    m_camera.position.z = cosf(angle) * 3.0f;
    m_camera.position.y = 2.0f;
}

void Renderer::UpdateConstantBuffer()
{
    XMVECTOR eye = XMLoadFloat3(&m_camera.position);
    XMVECTOR at = XMLoadFloat3(&m_camera.target);
    XMVECTOR up = XMLoadFloat3(&m_camera.up);

    XMMATRIX view = XMMatrixLookAtLH(eye, at, up);
    XMMATRIX proj = XMMatrixPerspectiveFovLH(m_camera.fov, m_camera.aspect, m_camera.nearZ, m_camera.farZ);
    XMMATRIX world = XMMatrixIdentity();

    m_cbData.mvp = XMMatrixTranspose(world * view * proj);

    m_context->UpdateSubresource(m_constantBuffer, 0, nullptr, &m_cbData, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void Renderer::Render(float dt)
{
    //UpdateCamera(dt);
    UpdateConstantBuffer();

    float clearColor[4] = { 0.1f, 0.1f, 0.2f, 1.0f };
    m_context->ClearRenderTargetView(m_renderTargetView, clearColor);
    m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    m_context->IASetInputLayout(m_inputLayout);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_context->IASetVertexBuffers(0, 1, &m_mesh.vertexBuffer, &stride, &offset);
    m_context->IASetIndexBuffer(m_mesh.indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    m_context->RSSetState(m_rasterizerState);
    m_context->OMSetDepthStencilState(m_depthState, 0);

    m_context->VSSetShader(m_vertexShader, nullptr, 0);
    m_context->PSSetShader(m_pixelShader, nullptr, 0);
    m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);

    m_context->DrawIndexed(m_mesh.indexCount, 0, 0);
    m_swapChain->Present(1, 0);
}

void Renderer::BeginFrame() {
    std::cout << "Begin Frame\n";
    float clearColor[4] = { 0.1f, 0.1f, 0.2f, 1.0f };
    m_context->ClearRenderTargetView(m_renderTargetView, clearColor);
    m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::EndFrame() {
    std::cout << "End Frame\n";
    m_swapChain->Present(1, 0);
}

void Renderer::DrawMesh(const XMMATRIX& world,
    ID3D11Buffer* vertexBuffer,
    ID3D11Buffer* indexBuffer,
    UINT indexCount)
{
    // Construire view et proj
    XMVECTOR eye = XMLoadFloat3(&m_camera.position);
    XMVECTOR at = XMLoadFloat3(&m_camera.target);
    XMVECTOR up = XMLoadFloat3(&m_camera.up);

    XMMATRIX view = XMMatrixLookAtLH(eye, at, up);
    XMMATRIX proj = XMMatrixPerspectiveFovLH(m_camera.fov, m_camera.aspect, m_camera.nearZ, m_camera.farZ);

    // Mettre à jour le constant buffer
    m_cbData.mvp = XMMatrixTranspose(world * view * proj);
    m_context->UpdateSubresource(m_constantBuffer, 0, nullptr, &m_cbData, 0, 0);

    // Bind pipeline
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    m_context->IASetInputLayout(m_inputLayout);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    m_context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    m_context->RSSetState(m_rasterizerState);
    m_context->OMSetDepthStencilState(m_depthState, 0);

    m_context->VSSetShader(m_vertexShader, nullptr, 0);
    m_context->PSSetShader(m_pixelShader, nullptr, 0);
    m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);

    // Draw
    m_context->DrawIndexed(indexCount, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/*void Renderer::DrawMesh(const XMMATRIX& world,
    ID3D11Buffer* vertexBuffer,
    ID3D11Buffer* indexBuffer,
    UINT indexCount)
{
    // 1. Construire view et proj comme dans UpdateConstantBuffer()
    XMVECTOR eye = XMLoadFloat3(&m_camera.position);
    XMVECTOR at = XMLoadFloat3(&m_camera.target);
    XMVECTOR up = XMLoadFloat3(&m_camera.up);

    XMMATRIX view = XMMatrixLookAtLH(eye, at, up);
    XMMATRIX proj = XMMatrixPerspectiveFovLH(m_camera.fov, m_camera.aspect, m_camera.nearZ, m_camera.farZ);

    // 2. Mettre à jour le constant buffer avec world * view * proj
    m_cbData.mvp = XMMatrixTranspose(world * view * proj);
    m_context->UpdateSubresource(m_constantBuffer, 0, nullptr, &m_cbData, 0, 0);

    // 3. Binder les buffers GPU
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    m_context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // 4. Binder shaders, input layout, rasterizer, depth state
    m_context->IASetInputLayout(m_inputLayout);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_context->RSSetState(m_rasterizerState);
    m_context->OMSetDepthStencilState(m_depthState, 0);
    m_context->VSSetShader(m_vertexShader, nullptr, 0);
    m_context->PSSetShader(m_pixelShader, nullptr, 0);
    m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);

    // 5. Draw call
    m_context->DrawIndexed(indexCount, 0, 0);
}*/

void Renderer::Shutdown()
{
    if (m_mesh.vertexBuffer) m_mesh.vertexBuffer->Release();
    if (m_mesh.indexBuffer)  m_mesh.indexBuffer->Release();

    if (m_constantBuffer)    m_constantBuffer->Release();
    if (m_inputLayout)       m_inputLayout->Release();
    if (m_vertexShader)      m_vertexShader->Release();
    if (m_pixelShader)       m_pixelShader->Release();

    if (m_rasterizerState)   m_rasterizerState->Release();
    if (m_depthState)        m_depthState->Release();

    if (m_depthStencilView)  m_depthStencilView->Release();
    if (m_depthStencilBuffer) m_depthStencilBuffer->Release();
    if (m_renderTargetView)  m_renderTargetView->Release();

    if (m_swapChain)         m_swapChain->Release();
    if (m_context)           m_context->Release();
    if (m_device)            m_device->Release();
}