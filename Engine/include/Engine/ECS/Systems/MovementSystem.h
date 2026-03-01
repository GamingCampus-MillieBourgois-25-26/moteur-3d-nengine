#pragma once
#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Velocity.h"
#include "MathsLib/Quaternion.h"

// Definition systeme 

class MovementSystem : public System
{
public:

    float rotationSpeedX = 0.0f;
    float rotationSpeedY = 1.0f;
    float rotationSpeedZ = 1.0f;

    void Update(Coordinator& coord, float dt);
};