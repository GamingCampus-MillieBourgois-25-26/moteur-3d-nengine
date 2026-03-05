#include "Engine/Scene/Scene.h"

Scene::Scene(const std::string& name, Renderer* renderer, ScriptManager* scriptManager)
    : m_name(name), m_renderer(renderer), m_scriptManager(scriptManager)
{
    m_coordinator.Init();

    // Components
    m_coordinator.RegisterComponent<Transform>();
    m_coordinator.RegisterComponent<MeshRenderer>();
    m_coordinator.RegisterComponent<MaterialData>();
    m_coordinator.RegisterComponent<Name>();
    m_coordinator.RegisterComponent<Script>();
    m_coordinator.RegisterComponent<Velocity>();

    // RenderSystem
    m_renderSystem = m_coordinator.RegisterSystem<RenderSystem>();
    Signature renderSig;
    renderSig.set(m_coordinator.GetComponentType<Transform>());
    renderSig.set(m_coordinator.GetComponentType<MeshRenderer>());
    renderSig.set(m_coordinator.GetComponentType<MaterialData>());
    m_coordinator.SetSystemSignature<RenderSystem>(renderSig);

    // MovementSystem
    m_movementSystem = m_coordinator.RegisterSystem<MovementSystem>();
    Signature moveSig;
    moveSig.set(m_coordinator.GetComponentType<Transform>());
    moveSig.set(m_coordinator.GetComponentType<Velocity>());
    m_coordinator.SetSystemSignature<MovementSystem>(moveSig);

    // ScriptSystem
    Signature scriptSig;
    scriptSig.set(m_coordinator.GetComponentType<Script>());
}

Entity Scene::CreateRenderableEntity()
{
    Entity e = m_coordinator.CreateEntity();
    m_entities.push_back(e);

    // Transform
    Transform t{};
    t.position = { 0,0,0 };
    t.scale = { 1,1,1 };
    t.rotation = { 0,0,0,1 };
    m_coordinator.AddComponent<Transform>(e, t);

    // Chargement du modele .obj
    OBJResult obj = LoadOBJ("OBJ/SpinCat.obj");

    // Création des buffers GPU
    MeshRenderer mr = m_renderer->CreateMeshRenderer(obj.mesh);
    m_coordinator.AddComponent<MeshRenderer>(e, mr);

    // Name
    Name name{};
    name.value = "Entity";
    m_coordinator.AddComponent<Name>(e, name);

    // Script
    Script script{};
    script.className = "Script";
    script.instance = nullptr;
    m_coordinator.AddComponent<Script>(e, script);

    // Velocity
    Velocity vel{};
    vel.velocity = { 0,0,0 };
    m_coordinator.AddComponent<Velocity>(e, vel);

    // Material
    MaterialData mat;
    mat.diffuse = m_renderer->CreateTextureFromFile(
        L"OBJ/" + std::wstring(obj.material.diffuseTexName.begin(), obj.material.diffuseTexName.end())
    );
    m_coordinator.AddComponent<MaterialData>(e, mat);

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
    if (m_movementSystem) m_movementSystem->Update(m_coordinator, dt);
}

void Scene::Render()
{
    if (m_renderSystem)
        m_renderSystem->Render(m_coordinator, *m_renderer);
}
