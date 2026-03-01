#pragma once
#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Velocity.h"
#include "Engine/ECS/Components/MeshRenderer.h"
#include "Engine/Renderer.h"
#include <DirectXMath.h>


class RenderSystem : public System
{
public:
    void Render(Coordinator& coord, Renderer& renderer);
};
