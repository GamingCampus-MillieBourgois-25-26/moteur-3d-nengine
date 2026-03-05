#pragma once
#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Velocity.h"
#include "Engine/ECS/Components/MeshRenderer.h"
#include "Engine/Renderer.h"
#include <DirectXMath.h>

/**
 * @brief Système de rendu des entités.
 *
 * Récupère les composants Transform et MeshRenderer,
 * construit la matrice monde et délègue le dessin
 * au Renderer.
 *
 * Ce système ne contient aucune logique de jeu.
 */
class RenderSystem : public System
{
public:
    /**
     * @brief Effectue le rendu de toutes les entités compatibles.
     * @param coord Coordinateur ECS
     * @param renderer Système de rendu
     */
    void Render(Coordinator& coord, Renderer& renderer);
};
