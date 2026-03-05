#pragma once
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