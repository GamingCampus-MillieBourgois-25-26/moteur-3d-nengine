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

    m_coordinator.RegisterComponent<Collider>();
    m_coordinator.RegisterComponent<Trigger>();
    m_coordinator.RegisterComponent<Force>();
    m_coordinator.RegisterComponent<Joint>();
    m_coordinator.RegisterComponent<CharacterController>();

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

    // 2. Register PhysicsBodySystem (owns the Bullet world + simulation)
    customPH.physicsBodySystem = m_coordinator.RegisterSystem<PhysicsBodySystem>();

    Signature physicsSignature;
    physicsSignature.set(m_coordinator.GetComponentType<Transform>(), true);
    m_coordinator.SetSystemSignature<PhysicsBodySystem>(physicsSignature);

    customPH.physicsBodySystem->Init();

    // 3. Register ColliderSystem (creates shapes → delegates to PhysicsBodySystem)
    customPH.colliderSystem = m_coordinator.RegisterSystem<ColliderSystem>();

    Signature colliderSignature;
    colliderSignature.set(m_coordinator.GetComponentType<Transform>(), true);
    colliderSignature.set(m_coordinator.GetComponentType<Collider>(), true);
    m_coordinator.SetSystemSignature<ColliderSystem>(colliderSignature);

    customPH.colliderSystem->Init(customPH.physicsBodySystem);

    // 4. Register ForceSystem (applies forces/impulses → delegates to PhysicsBodySystem)
    customPH.forceSystem = m_coordinator.RegisterSystem<ForceSystem>();

    Signature forceSignature;
    forceSignature.set(m_coordinator.GetComponentType<Force>(), true);
    m_coordinator.SetSystemSignature<ForceSystem>(forceSignature);

    customPH.forceSystem->Init(customPH.physicsBodySystem);

    // 5. Register JointSystem (creates constraints between entities)
    customPH.jointSystem = m_coordinator.RegisterSystem<JointSystem>();

    Signature jointSignature;
    jointSignature.set(m_coordinator.GetComponentType<Joint>(), true);
    m_coordinator.SetSystemSignature<JointSystem>(jointSignature);


    customPH.jointSystem->Init(customPH.physicsBodySystem);

    // 6. Register CharacterControllerSystem (manages playable characters)
    customPH.characterControllerSystem = m_coordinator.RegisterSystem<CharacterControllerSystem>();

    Signature ccSignature;
    ccSignature.set(m_coordinator.GetComponentType<Transform>(), true);
    ccSignature.set(m_coordinator.GetComponentType<CharacterController>(), true);
    m_coordinator.SetSystemSignature<CharacterControllerSystem>(ccSignature);

    customPH.characterControllerSystem->Init(customPH.physicsBodySystem);

    // 9. Register TriggerSystem (for managing trigger events)
    customPH.triggerSystem = m_coordinator.RegisterSystem<TriggerSystem>();

    Signature triggerSignature;
    triggerSignature.set(m_coordinator.GetComponentType<Transform>(), true);
    triggerSignature.set(m_coordinator.GetComponentType<Collider>(), true);
    triggerSignature.set(m_coordinator.GetComponentType<Trigger>(), true);
    m_coordinator.SetSystemSignature<TriggerSystem>(triggerSignature);

    customPH.triggerSystem->Init(customPH.physicsBodySystem);

    // 11. Create a trigger entity
    Entity triggerEntity = m_coordinator.CreateEntity();

    Transform triggerTr;
    triggerTr.position = { 1, 0, 1 };
    triggerTr.scale = { 1, 1, 1 };
    triggerTr.rotation = { 0, 0, 0, 1 };
    m_coordinator.AddComponent(triggerEntity, triggerTr);

    Collider triggerCol;
    triggerCol.shapeType = ColliderShapeType::Box;
    triggerCol.halfExtents = { 1.0f, 1.0f, 1.0f };
    triggerCol.mass = 0.0f; // static
    m_coordinator.AddComponent(triggerEntity, triggerCol);

    Trigger trigger;
    m_coordinator.AddComponent(triggerEntity, trigger);

    // Create a zone entity
    Entity zone = m_coordinator.CreateEntity();

    Transform zoneTransform;
    zoneTransform.position = { 0, 1, 0 };
    zoneTransform.scale = { 2, 2, 2 };
    zoneTransform.rotation = { 0, 0, 0, 1 };
    m_coordinator.AddComponent(zone, zoneTransform);

    Collider zoneCollider;
    zoneCollider.shapeType = ColliderShapeType::Box;
    zoneCollider.halfExtents = { 2.0f, 2.0f, 2.0f };
    zoneCollider.mass = 0.0f;
    m_coordinator.AddComponent(zone, zoneCollider);

    Trigger trig;
    trig.onEnter = [](Entity self, Entity other) {
        std::cout << "Entity " << other << " entered zone " << self << "\n";
        };
    trig.onExit = [](Entity self, Entity other) {
        std::cout << "Entity " << other << " left zone " << self << "\n";
        };
    m_coordinator.AddComponent(zone, trig);

    // Ground plane (static)
    Entity ground = m_coordinator.CreateEntity();

    Transform groundTr;
    groundTr.position = { 0, -1, 0 };
    groundTr.scale = { 1, 1, 1 };
    groundTr.rotation = { 0, 0, 0, 1 };
    m_coordinator.AddComponent(ground, groundTr);

    Collider groundCol;
    groundCol.shapeType = ColliderShapeType::Box;
    groundCol.halfExtents = { 50.0f, 1.0f, 50.0f };
    groundCol.mass = 0.0f; // static
    m_coordinator.AddComponent(ground, groundCol);
}

Entity Scene::CreateRenderableEntity()
{
    Entity e = m_coordinator.CreateEntity();
    m_entities.push_back(e);

    // Transform
    Transform t{};
    t.position = { 0,0,0 };
    t.scale = { 1,1,1 };
    t.rotation = { 0,0,180,1 };
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

    // Collider → ColliderSystem creates the shape, PhysicsBodySystem gets the rigid body
    Collider col;
    col.shapeType = ColliderShapeType::Box;
    col.halfExtents = { 0.5f, 0.5f, 0.5f };
    col.mass = 1.0f;
    m_coordinator.AddComponent(e, col);

    // Force → ForceSystem applies forces/impulses on the rigid body
    Force force;
    force.force = { 0.0f, 0.0f, 0.0f }; // No continuous linear force
    force.torque = { 0.0f, 3.f, 0.0f }; // Gentle continuous torque around Y axis
    force.mode = ForceMode::Force;      // Continuous force (not impulse)
    force.active = true;
    m_coordinator.AddComponent(e, force);

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

    if (customPH.colliderSystem)
        customPH.colliderSystem->Update(m_coordinator);

    if (customPH.jointSystem)
        customPH.jointSystem->Update(m_coordinator);

    if (customPH.forceSystem)
        customPH.forceSystem->Update(m_coordinator);

    if (customPH.characterControllerSystem)
        customPH.characterControllerSystem->Update(m_coordinator, dt);

    if (customPH.physicsBodySystem)
        customPH.physicsBodySystem->Update(m_coordinator, dt);

    if (customPH.triggerSystem)
        customPH.triggerSystem->Update(m_coordinator);

    if (m_movementSystem)
        m_movementSystem->Update(m_coordinator, dt);
}

void Scene::Render()
{
    if (m_renderSystem)
        m_renderSystem->Render(m_coordinator, *m_renderer);
}
