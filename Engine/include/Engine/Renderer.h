#pragma once
/**
 * @file Renderer.h
 * @brief Direct3D 11 renderer – device management, camera, mesh and texture helpers.
 * @ingroup Engine
 */

#include "Engine/ECS/Components/MeshRenderer.h"
#include "Engine/OBJ/WICTextureLoader.h"
#include "Engine/OBJ/NewOBJLoader.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

struct GLFWwindow;

/**
 * @brief Encapsulates the Direct3D 11 rendering pipeline.
 *
 * Responsibilities:
 *  - Create the D3D11 device, swap chain and render targets.
 *  - Load HLSL vertex and pixel shaders from disk.
 *  - Manage a simple perspective camera with yaw/pitch FPS controls.
 *  - Expose per-frame BeginFrame() / EndFrame() and DrawMesh() calls.
 *  - Create GPU buffers for meshes loaded via NewOBJLoader.
 *  - Load textures via the WIC texture loader.
 *
 * The Renderer is owned by Application and shared with the RenderSystem
 * via a reference.
 */
class Renderer
{
private:
    // ── Internal types ────────────────────────────────────────────────────

    /// @brief Interleaved vertex layout: position + normal + UV.
    struct Vertex
    {
        float x, y, z;   ///< World-space position.
        float nx, ny, nz; ///< Surface normal.
        float u, v;       ///< Texture coordinates.
    };

    /// @brief Per-object constant buffer data uploaded to the GPU each draw.
    struct ConstantBufferData
    {
        DirectX::XMMATRIX world; ///< Object-to-world transform.
        DirectX::XMMATRIX view;  ///< World-to-camera transform.
        DirectX::XMMATRIX proj;  ///< Camera-to-clip projection.
    };

    /// @brief Simple first-person camera state.
    struct Camera
    {
        DirectX::XMFLOAT3 position{0.0f, 0.0f, -3.0f}; ///< World-space position.
        DirectX::XMFLOAT3 target  {0.0f, 0.0f,  0.0f}; ///< Look-at target.
        DirectX::XMFLOAT3 up      {0.0f, 1.0f,  0.0f}; ///< World up vector.
        float fov    = DirectX::XM_PIDIV4; ///< Vertical FOV (radians).
        float aspect = 1.0f;               ///< Viewport aspect ratio (w/h).
        float nearZ  = 0.1f;               ///< Near clipping plane.
        float farZ   = 100.0f;             ///< Far clipping plane.
        float yaw    = 0.0f;               ///< Horizontal rotation (radians).
        float pitch  = 0.0f;              ///< Vertical rotation (radians).
    };

    // ── Private helpers ───────────────────────────────────────────────────

    bool CreateDeviceAndSwapChain(GLFWwindow* window, int width, int height);
    bool CreateRenderTargets(int width, int height);
    bool LoadShadersFromFiles(const std::wstring& vsPath, const std::wstring& psPath);
    bool CreatePipelineState();
    bool CreateMesh();
    void UpdateCamera(float dt);
    void UpdateConstantBuffer();

    // ── D3D11 objects ─────────────────────────────────────────────────────

    ID3D11Device*            m_device           = nullptr; ///< D3D11 logical device.
    ID3D11DeviceContext*     m_context          = nullptr; ///< Immediate device context.
    IDXGISwapChain*          m_swapChain        = nullptr; ///< DXGI swap chain.
    ID3D11RenderTargetView*  m_renderTargetView = nullptr; ///< Back-buffer RTV.
    ID3D11Texture2D*         m_depthStencilBuffer = nullptr; ///< Depth/stencil texture.
    ID3D11DepthStencilView*  m_depthStencilView   = nullptr; ///< DSV for depth testing.
    ID3D11VertexShader*      m_vertexShader     = nullptr; ///< Compiled vertex shader.
    ID3D11PixelShader*       m_pixelShader      = nullptr; ///< Compiled pixel shader.
    ID3D11InputLayout*       m_inputLayout      = nullptr; ///< Vertex input layout.
    ID3D11Buffer*            m_constantBuffer   = nullptr; ///< Per-draw constant buffer.
    ID3D11RasterizerState*   m_rasterizerState  = nullptr; ///< Rasteriser settings.
    ID3D11DepthStencilState* m_depthState       = nullptr; ///< Depth/stencil settings.

    Camera             m_camera  = {};   ///< Active camera state.
    ConstantBufferData m_cbData  = {};   ///< CPU-side constant buffer mirror.
    int                m_width   = 0;    ///< Viewport width in pixels.
    int                m_height  = 0;    ///< Viewport height in pixels.

public:
    /**
     * @brief Initialises the full D3D11 pipeline for the given GLFW window.
     * @param window  GLFW window that provides the native HWND.
     * @param width   Initial viewport width.
     * @param height  Initial viewport height.
     * @return true on success.
     */
    bool Initialize(GLFWwindow* window, int width, int height);

    /**
     * @brief Renders all loaded objects (legacy single-mesh path).
     * @param dt  Delta-time in seconds.
     */
    void Render(float dt);

    /// @brief Releases all D3D11 resources.
    void Shutdown();

    // ── Camera control ────────────────────────────────────────────────────

    /**
     * @brief Translates the camera along its local axes.
     * @param dx  Right/left delta.
     * @param dy  Up/down delta.
     * @param dz  Forward/backward delta.
     */
    void MoveCamera(float dx, float dy, float dz);

    /**
     * @brief Rotates the camera.
     * @param yaw    Horizontal rotation delta (radians).
     * @param pitch  Vertical rotation delta (radians).
     */
    void RotateCamera(float yaw, float pitch);

    /**
     * @brief Enables or disables camera mouse capture.
     * @param capture  true to hide and lock the cursor.
     */
    void SetCameraCapture(bool capture);

    /**
     * @brief Handles window resize events (recreates swap-chain buffers).
     * @param newWidth   New viewport width in pixels.
     * @param newHeight  New viewport height in pixels.
     */
    void OnResize(int newWidth, int newHeight);

    // ── Frame control ─────────────────────────────────────────────────────

    /// @brief Clears the back buffer and depth buffer (call at the start of each frame).
    void BeginFrame();

    /// @brief Presents the swap chain (call at the end of each frame).
    void EndFrame();

    // ── Draw helpers ──────────────────────────────────────────────────────

    /**
     * @brief Draws a mesh with the given world transform and GPU buffers.
     * @param world         World-space transform matrix.
     * @param vertexBuffer  GPU vertex buffer.
     * @param indexBuffer   GPU index buffer.
     * @param indexCount    Number of indices to draw.
     */
    void DrawMesh(const DirectX::XMMATRIX& world,
                  ID3D11Buffer* vertexBuffer,
                  ID3D11Buffer* indexBuffer,
                  UINT indexCount);

    /**
     * @brief Uploads CPU mesh data to the GPU and returns a MeshRenderer component.
     * @param mesh  CPU-side mesh data from NewOBJLoader.
     * @return MeshRenderer component ready to be attached to an entity.
     */
    MeshRenderer CreateMeshRenderer(const MeshData& mesh);

    /**
     * @brief Loads a texture from disk and returns a shader-resource view.
     * @param path  Wide-string file path (PNG, JPG, DDS, etc.).
     * @return Non-owning SRV pointer (lifetime managed by the Renderer).
     */
    ID3D11ShaderResourceView* CreateTextureFromFile(const std::wstring& path);

    ID3D11SamplerState* m_sampler = nullptr; ///< Default texture sampler.
    OBJResult           m_objresult;          ///< Last loaded OBJ mesh result.

    /// @brief Returns the D3D11 device (non-owning).
    ID3D11Device*        GetDevice()  const { return m_device; }

    /// @brief Returns the immediate device context (for ImGui or external rendering).
    ID3D11DeviceContext* GetContext() const { return m_context; }

    /// @brief Returns the last loaded mesh result.
    const OBJResult& GetMesh() const { return m_objresult; }
};
