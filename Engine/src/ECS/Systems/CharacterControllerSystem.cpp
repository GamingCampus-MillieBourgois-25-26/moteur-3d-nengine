#include "Engine/ECS/Systems/CharacterControllerSystem.h"
#include <iostream>

void CharacterControllerSystem::Init(std::shared_ptr<PhysicsBodySystem> physicsSystem)
{
    m_physicsSystem = physicsSystem;
    std::cout << "CharacterControllerSystem initialized.\n";
}

void CharacterControllerSystem::CreateController(Entity entity, Coordinator& coord)
{
    auto& cc        = coord.GetComponent<CharacterController>(entity);
    auto& transform = coord.GetComponent<Transform>(entity);

    btDynamicsWorld* world = m_physicsSystem->GetDynamicsWorld();
    if (!world)
        return;

    // Capsule shape
    auto shape = std::make_unique<btCapsuleShape>(cc.capsuleRadius, cc.capsuleHeight);

    // Ghost object (detecte les overlaps sans etre un rigid body)
    auto ghostObject = std::make_unique<btPairCachingGhostObject>();

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

    ghostObject->setWorldTransform(startTransform);
    ghostObject->setCollisionShape(shape.get());
    ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    ghostObject->setUserIndex(static_cast<int>(entity));

    // Character controller
    auto controller = std::make_unique<btKinematicCharacterController>(
        ghostObject.get(), shape.get(), cc.stepHeight, btVector3(0.0f, 1.0f, 0.0f));

    controller->setGravity(btVector3(0.0f, cc.gravity, 0.0f));
    controller->setJumpSpeed(cc.jumpSpeed);
    controller->setMaxSlope(cc.maxSlopeAngle);

    // Ajouter le ghost object et le controller au monde Bullet
    world->addCollisionObject(ghostObject.get(),
        btBroadphaseProxy::CharacterFilter,
        btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

    world->addAction(controller.get());

    ControllerData data;
    data.controller  = std::move(controller);
    data.ghostObject = std::move(ghostObject);
    data.shape       = std::move(shape);

    m_controllers[entity] = std::move(data);

    cc.initialized = true;

    std::cout << "CharacterControllerSystem: created controller for entity " << entity << "\n";
}

void CharacterControllerSystem::Update(Coordinator& coord, float dt)
{
    if (!m_physicsSystem)
        return;

    for (auto entity : mEntities)
    {
        auto& cc = coord.GetComponent<CharacterController>(entity);

        // Creer le controller si pas encore fait
        if (!cc.initialized)
        {
            CreateController(entity, coord);
            if (!cc.initialized)
                continue;
        }

        auto it = m_controllers.find(entity);
        if (it == m_controllers.end())
            continue;

        auto& data = it->second;

        // Appliquer la direction de deplacement
        btVector3 walkDir(
            cc.walkDirection.x * cc.walkSpeed * dt,
            0.0f,
            cc.walkDirection.z * cc.walkSpeed * dt
        );
        data.controller->setWalkDirection(walkDir);

        // Saut
        if (cc.jump && data.controller->onGround())
        {
            data.controller->jump();
            cc.jump = false;
        }

        // Mettre a jour l'etat "au sol"
        cc.onGround = data.controller->onGround();

        // Synchroniser la position du ghost object vers le Transform ECS
        btTransform btTrans = data.ghostObject->getWorldTransform();

        auto& transform = coord.GetComponent<Transform>(entity);

        const btVector3& origin = btTrans.getOrigin();
        transform.position.x = origin.getX();
        transform.position.y = origin.getY();
        transform.position.z = origin.getZ();

        const btQuaternion& rot = btTrans.getRotation();
        transform.rotation.x = rot.getX();
        transform.rotation.y = rot.getY();
        transform.rotation.z = rot.getZ();
        transform.rotation.w = rot.getW();
    }
}

void CharacterControllerSystem::RemoveController(Entity entity)
{
    auto it = m_controllers.find(entity);
    if (it == m_controllers.end())
        return;

    btDynamicsWorld* world = m_physicsSystem ? m_physicsSystem->GetDynamicsWorld() : nullptr;
    if (world)
    {
        world->removeAction(it->second.controller.get());
        world->removeCollisionObject(it->second.ghostObject.get());
    }

    m_controllers.erase(it);
}

void CharacterControllerSystem::Shutdown()
{
    btDynamicsWorld* world = m_physicsSystem ? m_physicsSystem->GetDynamicsWorld() : nullptr;

    for (auto& [entity, data] : m_controllers)
    {
        if (world)
        {
            world->removeAction(data.controller.get());
            world->removeCollisionObject(data.ghostObject.get());
        }
    }
    m_controllers.clear();

    std::cout << "CharacterControllerSystem shut down.\n";
}