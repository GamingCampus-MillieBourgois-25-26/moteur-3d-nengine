#include "Engine/ECS/Systems/RenderSystem.h"
#include <DirectXMath.h>

void RenderSystem::Render(Coordinator& coord, Renderer& renderer)
{
    // Nettoyage du frame buffer
    renderer.BeginFrame();

    // mEntities contient toutes les entités ayant Transform + MeshRenderer
    for (auto entity : mEntities)
    {
        // Recuperer Transform 
        auto& tr = coord.GetComponent<Transform>(entity);

        // Recuperer MeshRenderer
        auto& mr = coord.GetComponent<MeshRenderer>(entity);

        //Construire la matrice world
        DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(tr.scale.x, tr.scale.y, tr.scale.z);
        DirectX::XMVECTOR rot = DirectX::XMVectorSet(tr.rotation.x, tr.rotation.y, tr.rotation.z, tr.rotation.w);
        DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationQuaternion(rot);
        DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslation(tr.position.x, tr.position.y, tr.position.z);

        // world = scale * rotation * translation
        DirectX::XMMATRIX world = scaleMat * rotMat * transMat;

        // Envoi au renderer
        renderer.DrawMesh(world, mr.vertexBuffer, mr.indexBuffer, mr.indexCount);
    }

    // Présentation à l'écran
    renderer.EndFrame();
}