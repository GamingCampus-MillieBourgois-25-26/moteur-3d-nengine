/*#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <d3d11.h>
#include "Engine/Renderer.h"


class OBJLoader {
private:
  
	struct Vec3 { float x, y, z; Vec3 operator*(float s) const { return { x * s, y * s, z * s }; }};
	struct Vec2 { float u, v; };
	struct Vertex { Vec3 position; Vec2 uv; Vec3 normal; };
	struct VertexKey {
		int pos, uv, norm;

		bool operator<(const VertexKey& other) const
		{
			return std::tie(pos, uv, norm) < std::tie(other.pos, other.uv, other.norm);
		}
	};
	struct FaceIndex { int pos, uv, norm; };

	std::string line;
	std::ifstream file;

	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	std::map<VertexKey, uint32_t> vertexCache;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr;
	Renderer rend;
	UINT indexCount;
	ID3D11Device* m_device = nullptr;

public:

	void setDevice(ID3D11Device* d);
	void loadOBJFile();
	FaceIndex cutFace(std::string sg);
	uint32_t addVertex(FaceIndex fix);
	void vertexBufferCreation();
	void indexBufferCreation();

	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();
	UINT getIndexCount();
};*/