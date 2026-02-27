#include "Engine/ECS/Systems/RenderSystem.h"

void RenderSystem::Render(Coordinator& coord, Renderer& renderer)
{
    for (auto entity : mEntities)
    {
        // Récupérer Transform 
        auto& tr = coord.GetComponent<Transform>(entity);
        auto& mr = coord.GetComponent<MeshRenderer>(entity);
        //Transform tr;

        // Récupérer MeshRenderer
        //MeshRenderer mr;

        //Construire la matrice world
        world =
            scaleMat.scale(tr.scale) *
            quaternionMat.rotate(tr.rotation) *
            translationMat.translate(tr.position);

        // Appeler renderer.DrawMesh(...)
        renderer.DrawMesh(world, mr.vertexBuffer, mr.indexBuffer, mr.indexCount);
    }
}