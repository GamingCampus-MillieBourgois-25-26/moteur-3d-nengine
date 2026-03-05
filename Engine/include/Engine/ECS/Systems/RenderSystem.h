#pragma once
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

/*
    RenderSystem
    ------------
    Système de rendu :
    - récupère Transform + MeshRenderer
    - construit la matrice world
    - appelle le Renderer pour dessiner

    Ce système ne fait aucune logique de jeu.
*/

class RenderSystem : public System
{
public:
    void Render(Coordinator& coord, Renderer& renderer);
};
