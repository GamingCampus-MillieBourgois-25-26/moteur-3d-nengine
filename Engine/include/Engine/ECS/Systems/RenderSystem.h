#pragma once
#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/MeshRenderer.h"
#include "Engine/Renderer.h"
#include "MathsLib/Matrice4.h"
#include "MathsLib/Quaternion.h"
#include "MathsLib/Vector3.h"


class RenderSystem : public System
{
public:
    void Render(Coordinator& coord, Renderer& renderer);

    MathsLib::Matrice4<float> scaleMat;
    MathsLib::Matrice4<float> quaternionMat;
    MathsLib::Matrice4<float> translationMat;
    MathsLib::Matrice4<float> world;


    /*
    
    MathsLib::Matrix4<float> Matrix4::CreateScale(const Vector3<float>& s);
    MathsLib::Matrix4<float> Matrix4::CreateFromQuaternion(const Quaternion<float>& q);
    MathsLib::Matrix4<float> Matrix4::CreateTranslation(const Vector3<float>& t);

    
    */
};
