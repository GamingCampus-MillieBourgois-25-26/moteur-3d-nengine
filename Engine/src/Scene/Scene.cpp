#include "Engine/Scene/Scene.h"

Scene::Scene(const std::string& name, Renderer* renderer, ScriptManager* scriptManager)
    : m_name(name)
    , m_renderer(renderer)
    , m_scriptManager(scriptManager)
{
    m_coordinator.Init();

    // Enregistrer les composants
    m_coordinator.RegisterComponent<Transform>();
    m_coordinator.RegisterComponent<MeshRenderer>();
    m_coordinator.RegisterComponent<Name>();
    m_coordinator.RegisterComponent<Script>();

    // Enregistrer RenderSystem
    m_renderSystem = m_coordinator.RegisterSystem<RenderSystem>();
    Signature renderSig;
    renderSig.set(m_coordinator.GetComponentType<Transform>());
    renderSig.set(m_coordinator.GetComponentType<MeshRenderer>());
    m_coordinator.SetSystemSignature<RenderSystem>(renderSig);
}

Entity Scene::CreateRenderableEntity()
{
    Entity e = m_coordinator.CreateEntity();
    m_entities.push_back(e);

    // Transform
    tr.position = { 0, 0, 0 };
    tr.scale = { 1, 1, 1 };
    tr.rotation = { 0, 0, 0, 1 };
    m_coordinator.AddComponent<Transform>(e, tr);

    // MeshRenderer
    mr.vertexBuffer = m_renderer->GetMesh().vertexBuffer;
    mr.indexBuffer = m_renderer->GetMesh().indexBuffer;
    mr.indexCount = m_renderer->GetMesh().indexCount;
    m_coordinator.AddComponent<MeshRenderer>(e, mr);

    // ⭐ AJOUTER Name
    name.value = "Entity";
    m_coordinator.AddComponent<Name>(e, name);

    // ⭐ AJOUTER Script
    script.className = "Script";
    script.instance = nullptr;
    m_coordinator.AddComponent<Script>(e, script);

    return e;
}

void Scene::DestroyEntity(Entity e)
{
    m_coordinator.DestroyEntity(e);
    m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), e), m_entities.end());
}

Transform Scene::GetTransform(Entity e)
{
    return m_coordinator.GetComponent<Transform>(e);
}

void Scene::SetTransform(Entity e, const Transform& t)
{
    m_coordinator.GetComponent<Transform>(e) = t;
}

void Scene::Update(float dt)
{
    // plus tard : ScriptSystem, PhysicsSystem, etc.
    (void)dt;
}

void Scene::Render()
{
    if (m_renderSystem)
        m_renderSystem->Render(m_coordinator, *m_renderer);
}
