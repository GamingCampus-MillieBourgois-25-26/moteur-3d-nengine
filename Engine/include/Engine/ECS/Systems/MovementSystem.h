#pragma once
#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Velocity.h"

// Definition systeme 

class MovementSystem : public System
{
public:
    void Update(Coordinator& coord);
};