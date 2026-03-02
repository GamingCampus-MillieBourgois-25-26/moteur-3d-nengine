#pragma once
#include <d3d11.h>

struct MeshRenderer { // comme le mesh dans entity mais sans logique
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	UINT indexCount;
};