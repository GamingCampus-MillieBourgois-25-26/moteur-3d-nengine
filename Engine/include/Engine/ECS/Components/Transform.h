#pragma once
#include "MathsLib/Vector3.h"
#include "MathsLib/Quaternion.h"
 
//Definition d'un composant

struct Transform 
{ 
	Transform() = default;
	MathsLib::Vector3<float> position; 
	MathsLib::Quaternion<float> rotation; 
	MathsLib::Vector3<float> scale; 
};