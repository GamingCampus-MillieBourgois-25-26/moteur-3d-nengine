#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class OBJLoader {

public : 

	void loadOBJFile();

private : 

	struct Vec3 { float x, y, z; }; 
	struct Vec2 { float u, v; };

	std::string line;
	std::ifstream file;

	std::vector<Vec3> positions; 
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs; 
};