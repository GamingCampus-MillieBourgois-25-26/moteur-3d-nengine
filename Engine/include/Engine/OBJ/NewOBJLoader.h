#pragma once
#include "Engine/OBJ/tiny_obj_loader.h"
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

// Charge un fichier .obj et reconstruit un mesh optimisé (vertices uniques + indices) 
MeshData LoadOBJ(const std::string& path);