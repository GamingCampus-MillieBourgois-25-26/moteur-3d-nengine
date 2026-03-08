#pragma once
/**
 * @file NewOBJLoader.h
 * @brief Engine-specific OBJ mesh loader that wraps tinyobj and uploads geometry to D3D11.
 *
 * @note This is a third-party or platform-specific header.
 *       Refer to the original library documentation for detailed API information.
 */

#include "Engine/OBJ/tiny_obj_loader.h"
#include "Engine/OBJ/WICTextureLoader.h"
#include "Engine/ECS/Components/MaterialData.h"
#include "DirectXMath.h"

#include <unordered_map>
#include <iostream>

struct Vertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 uv;
};

struct MeshData {
    std::vector<Vertex> vertices; // Vertex buffer "logique"
    std::vector<uint32_t> indices; // Index buffer
};

struct MaterialCPU {
    std::string diffuseTexName;
};

struct OBJResult {
    MeshData mesh;
    MaterialCPU material;
};

// Charge un fichier .obj et reconstruit un mesh optimisé (vertices uniques + indices) 
OBJResult LoadOBJ(const std::string& path);