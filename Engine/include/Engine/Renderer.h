#pragma once
#include "Engine/ECS/Components/MeshRenderer.h"
#include "Engine/OBJ/NewOBJLoader.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <iostream>

struct GLFWwindow;

/**
 * @brief Système de rendu basé sur DirectX 11.
 *
 * Gère l'initialisation du pipeline graphique, la caméra,
 * le rendu des meshes et l'intégration avec l'ECS.
 */
class Renderer
{
public:
    /**
     * @brief Initialise le système de rendu.
     * @param window Fenêtre GLFW associée
     * @param width Largeur de la fenêtre
     * @param height Hauteur de la fenêtre
     * @return true si l'initialisation a réussi
     */
    bool Initialize(GLFWwindow* window, int width, int height);

    /**
     * @brief Effectue le rendu d'une frame.
     * @param dt Temps écoulé depuis la dernière frame
     */
    void Render(float dt);

    /**
     * @brief Libère les ressources du système de rendu.
     */
    void Shutdown();

    /**
     * @brief Déplace la caméra dans l'espace.
     * @param dx Déplacement sur l'axe X
     * @param dy Déplacement sur l'axe Y
     * @param dz Déplacement sur l'axe Z
     */
    void MoveCamera(float dx, float dy, float dz);

    /**
     * @brief Fait pivoter la caméra.
     * @param yaw Rotation horizontale
     * @param pitch Rotation verticale
     */
    void RotateCamera(float yaw, float pitch);

    /**
     * @brief Active ou désactive la capture de la caméra.
     * @param capture État de la capture
     */
    void SetCameraCapture(bool capture);

    /** @brief Prépare le début du rendu d'une frame. */
    void BeginFrame();

    /** @brief Finalise le rendu d'une frame. */
    void EndFrame();

    /**
     * @brief Dessine un mesh à l'écran.
     * @param world Matrice de transformation monde
     * @param vertexBuffer Buffer de sommets
     * @param indexBuffer Buffer d'indices
     * @param indexCount Nombre d'indices
     */
    void DrawMesh(const DirectX::XMMATRIX& world,
        ID3D11Buffer* vertexBuffer,
        ID3D11Buffer* indexBuffer,
        UINT indexCount);

    /**
     * @brief Récupère le périphérique DirectX.
     * @return Pointeur vers le device DirectX
     */
    ID3D11Device* GetDevice() const { return m_device; }

private:
    /**
     * @brief Structure représentant un sommet.
     */
    struct Vertex
    {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
    };

    /**
     * @brief Données du constant buffer.
     */
    struct ConstantBufferData
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX proj;
    };

    /**
     * @brief Structure représentant la caméra.
     */
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
    bool CreateMesh();
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

    Camera             m_camera;
    ConstantBufferData m_cbData;

    int m_width = 0;
    int m_height = 0;

public:
    /**
     * @brief Crée un composant MeshRenderer à partir de données de mesh.
     * @param mesh Données du mesh
     * @return Composant MeshRenderer créé
     */
    MeshRenderer CreateMeshRenderer(const MeshData& mesh);
};
