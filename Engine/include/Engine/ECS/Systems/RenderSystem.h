#pragma once
/**
 * @file RenderSystem.h
 * @brief System that submits mesh draw calls to the Renderer.
 * @ingroup Systems
 */

#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Velocity.h"
#include "Engine/ECS/Components/MeshRenderer.h"
#include "Engine/ECS/Components/MaterialData.h"
#include "Engine/ECS/Components/Name.h"
#include "Engine/ECS/Components/Script.h"
#include "Engine/Renderer.h"
#include <DirectXMath.h>

/**
 * @brief Converts Transform + MeshRenderer components into Renderer draw calls.
 *
 * Required components: **Transform** + **MeshRenderer**
 *
 * Each frame Render() constructs a world matrix from Transform and calls
 * Renderer::DrawMesh() with the entity's GPU buffers.  No game logic is
 * performed here.
 */
class RenderSystem : public System
{
public:
    /**
     * @brief Draws all registered entities using the provided Renderer.
     * @param coord    ECS coordinator for component access.
     * @param renderer Renderer instance used to issue draw calls.
     */
    void Render(Coordinator& coord, Renderer& renderer);
};
