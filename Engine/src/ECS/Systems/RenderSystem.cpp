#include "Engine/ECS/Systems/RenderSystem.h"
#include <DirectXMath.h>

void RenderSystem::Render(Coordinator & coord, Renderer & renderer)
{
    // Nettoyage du frame buffer
    renderer.BeginFrame();

    // mEntities contient toutes les entitÔøΩs ayant Transform + MeshRenderer
    for (auto entity : mEntities)
    {
        // Recuperer Transform 
        auto& tr = coord.GetComponent<Transform>(entity);

        // Recuperer MeshRenderer
        auto& mr = coord.GetComponent<MeshRenderer>(entity);

        // RÈcupÈrer le material
        auto& mat = coord.GetComponent<MaterialData>(entity);

        //Construire la matrice world
        DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(tr.scale.x, tr.scale.y, tr.scale.z);
        DirectX::XMVECTOR rot = DirectX::XMVectorSet(tr.rotation.x, tr.rotation.y, tr.rotation.z, tr.rotation.w);
        DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationQuaternion(rot);
        DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslation(tr.position.x, tr.position.y, tr.position.z);

        // world = scale * rotation * translation
        DirectX::XMMATRIX world = scaleMat * rotMat * transMat;

        if (mat.diffuse) {
            renderer.GetContext()->PSSetShaderResources(0, 1, &mat.diffuse);
        }
        else {
            std::wcout << L"[RENDER WARNING] Texture diffuse NULL pour l'entitÈ " << entity << std::endl;
        }

        renderer.GetContext()->PSSetSamplers(0, 1, &renderer.m_sampler);

        // Envoi au renderer
        renderer.DrawMesh(world, mr.vertexBuffer, mr.indexBuffer, mr.indexCount);
    }

    // Ne pas appeler EndFrame ici : l'ex√©cutable (qui g√®re ImGui) fera la pr√©sentation apr√®s avoir rendu l'UI.
}