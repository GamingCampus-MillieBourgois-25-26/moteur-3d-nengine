#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <iostream>

struct GLFWwindow;

class Renderer
{
public:
    bool Initialize(GLFWwindow* window, int width, int height);
    void Render(float dt);
    void Shutdown();

    // ECS Modif
    void MoveCamera(float dx, float dy, float dz);
    void RotateCamera(float yaw, float pitch);
    void SetCameraCapture(bool capture);
    void OnResize(int newWidth, int newHeight);
    void BeginFrame();
    void EndFrame();
    void DrawMesh(const DirectX::XMMATRIX& world, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT indexCount);

    ID3D11Device* GetDevice() const { return m_device; }
    ID3D11DeviceContext* GetContext() const { return m_context; } // ajouté pour ImGui DX11

private:
    struct Vertex
    {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
    };

    struct ConstantBufferData
    {
        //DirectX::XMMATRIX mvp;
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX proj;
    };

    struct Mesh
    {
        ID3D11Buffer* vertexBuffer = nullptr;
        ID3D11Buffer* indexBuffer = nullptr;
        UINT indexCount = 0;
    };

    struct Camera
    {
        DirectX::XMFLOAT3 position{ 0.0f, 0.0f, -3.0f };
        DirectX::XMFLOAT3 target{ 0.0f, 0.0f, 0.0f };
        DirectX::XMFLOAT3 up{ 0.0f, 1.0f, 0.0f };
        float fov = DirectX::XM_PIDIV4;
        float aspect = 1.0f;
        float nearZ = 0.1f;
        float farZ = 100.0f;
        float yaw = 0.0f;
        float pitch = 0.0f;
    };

private:
    bool CreateDeviceAndSwapChain(GLFWwindow* window, int width, int height);
    bool CreateRenderTargets(int width, int height);
    bool LoadShadersFromFiles(const std::wstring& vsPath, const std::wstring& psPath);
    bool CreatePipelineState();
    bool CreateMesh(); // mesh loader minimal (CPU → GPU)
    void UpdateCamera(float dt);
    void UpdateConstantBuffer();

private:
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;
    IDXGISwapChain* m_swapChain = nullptr;

    ID3D11RenderTargetView* m_renderTargetView = nullptr;
    ID3D11Texture2D* m_depthStencilBuffer = nullptr;
    ID3D11DepthStencilView* m_depthStencilView = nullptr;

    ID3D11VertexShader* m_vertexShader = nullptr;
    ID3D11PixelShader* m_pixelShader = nullptr;
    ID3D11InputLayout* m_inputLayout = nullptr;
    ID3D11Buffer* m_constantBuffer = nullptr;

    ID3D11RasterizerState* m_rasterizerState = nullptr;
    ID3D11DepthStencilState* m_depthState = nullptr;

    Mesh                    m_mesh;
    Camera                  m_camera;
    ConstantBufferData      m_cbData;

    int                     m_width = 0;
    int                     m_height = 0;

    // ECS
public:

    const Mesh& GetMesh() const { return m_mesh; }

};