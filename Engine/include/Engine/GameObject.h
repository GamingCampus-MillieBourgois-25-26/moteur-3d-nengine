#pragma once
#include <iostream>
#include "MathsLib/Quaternion.h"
#include "MathsLib/Vector3.h"

class GameObject {
private:
	float x, y, z;

public : 

	struct Transform { MathsLib::Vector3<float> position; MathsLib::Quaternion<float> rotation; MathsLib::Vector3<float> scale; };

	GameObject();
	~GameObject();



};