#pragma once
#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Velocity.h"
#include "MathsLib/Quaternion.h"

/*
    MovementSystem
    --------------
    Système logique qui met à jour :
    - la position (via Velocity)
    - la rotation (via des vitesses de rotation)

    Ce système traite uniquement les entités ayant :
    - Transform
    - Velocity
*/

class MovementSystem : public System
{
public:

    // Vitesses de rotation indépendantes sur chaque axe
    float rotationSpeedX = 0.0f;
    float rotationSpeedY = 0.0f;
    float rotationSpeedZ = 0.0f;

    // Met à jour toutes les entités compatibles
    void Update(Coordinator& coord, float dt);
};