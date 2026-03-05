#include "Engine/ECS/Systems/PhysicsBodySystem.h"
#include <iostream>

void PhysicsBodySystem::Init()
{
    m_collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
    m_dispatcher      = std::make_unique<btCollisionDispatcher>(m_collisionConfig.get());
    m_broadphase      = std::make_unique<btDbvtBroadphase>();
    m_solver          = std::make_unique<btSequentialImpulseConstraintSolver>();

    m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
        m_dispatcher.get(),
        m_broadphase.get(),
        m_solver.get(),
        m_collisionConfig.get()
    );

    m_dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));

    std::cout << "PhysicsBodySystem initialized.\n";
}

void PhysicsBodySystem::AddRigidBody(Entity entity, Coordinator& coord, float mass,
                                     std::unique_ptr<btCollisionShape> shape)
{
    // Ne pas ajouter deux fois le meme entity
    if (m_bodies.find(entity) != m_bodies.end())
        return;

    auto& transform = coord.GetComponent<Transform>(entity);

    // Position initiale depuis le Transform
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(
        transform.position.x,
        transform.position.y,
        transform.position.z
    ));
    startTransform.setRotation(btQuaternion(
        transform.rotation.x,
        transform.rotation.y,
        transform.rotation.z,
        transform.rotation.w
    ));

    // Inertie locale
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    if (mass > 0.0f)
        shape->calculateLocalInertia(mass, localInertia);

    auto motionState = std::make_unique<btDefaultMotionState>(startTransform);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(
        mass, motionState.get(), shape.get(), localInertia
    );

    auto rigidBody = std::make_unique<btRigidBody>(rbInfo);

    // Stocker l'entity dans le user index pour identification par le TriggerSystem
    rigidBody->setUserIndex(static_cast<int>(entity));

    m_dynamicsWorld->addRigidBody(rigidBody.get());

    BodyData data;
    data.rigidBody   = std::move(rigidBody);
    data.shape       = std::move(shape);
    data.motionState = std::move(motionState);

    m_bodies[entity] = std::move(data);
}

void PhysicsBodySystem::RemoveRigidBody(Entity entity)
{
    auto it = m_bodies.find(entity);
    if (it == m_bodies.end())
        return;

    m_dynamicsWorld->removeRigidBody(it->second.rigidBody.get());
    m_bodies.erase(it);
}

bool PhysicsBodySystem::HasRigidBody(Entity entity) const
{
    return m_bodies.find(entity) != m_bodies.end();
}

btRigidBody* PhysicsBodySystem::GetRigidBody(Entity entity) const
{
    auto it = m_bodies.find(entity);
    if (it != m_bodies.end())
        return it->second.rigidBody.get();
    return nullptr;
}

btDynamicsWorld* PhysicsBodySystem::GetDynamicsWorld() const
{
    return m_dynamicsWorld.get();
}

void PhysicsBodySystem::Update(Coordinator& coord, float dt)
{
    if (!m_dynamicsWorld)
        return;

    // Avancer la simulation Bullet
    m_dynamicsWorld->stepSimulation(dt, 10);

    // Synchroniser chaque rigid body vers Transform ECS
    for (auto& [entity, bodyData] : m_bodies)
    {
        btTransform btTrans;
        bodyData.rigidBody->getMotionState()->getWorldTransform(btTrans);

        auto& transform = coord.GetComponent<Transform>(entity);

        // Position
        const btVector3& origin = btTrans.getOrigin();
        transform.position.x = origin.getX();
        transform.position.y = origin.getY();
        transform.position.z = origin.getZ();

        // Rotation
        const btQuaternion& rot = btTrans.getRotation();
        transform.rotation.x = rot.getX();
        transform.rotation.y = rot.getY();
        transform.rotation.z = rot.getZ();
        transform.rotation.w = rot.getW();
    }
}

void PhysicsBodySystem::Shutdown()
{
    // Retirer tous les rigid bodies du monde
    for (auto& [entity, bodyData] : m_bodies)
    {
        m_dynamicsWorld->removeRigidBody(bodyData.rigidBody.get());
    }
    m_bodies.clear();

    m_dynamicsWorld.reset();
    m_solver.reset();
    m_broadphase.reset();
    m_dispatcher.reset();
    m_collisionConfig.reset();

    std::cout << "PhysicsBodySystem shut down.\n";
}