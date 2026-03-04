#include "Engine/ECS/Systems/ColliderSystem.h"
#include <iostream>

void ColliderSystem::Init(std::shared_ptr<PhysicsBodySystem> physicsSystem)
{
    m_physicsSystem = physicsSystem;
    std::cout << "ColliderSystem initialized.\n";
}

void ColliderSystem::Update(Coordinator& coord)
{
    if (!m_physicsSystem)
        return;

    // Parcourir les entites ayant Transform + Collider
    for (auto entity : mEntities)
    {
        auto& collider = coord.GetComponent<Collider>(entity);

        // Ne creer la shape qu'une seule fois
        if (collider.initialized)
            continue;

        // Verifier que le PhysicsBodySystem n'a pas deja un body pour cette entite
        if (m_physicsSystem->HasRigidBody(entity))
        {
            collider.initialized = true;
            continue;
        }

        // Creer la shape et deleguer au PhysicsBodySystem
        auto shape = CreateShape(collider);
        m_physicsSystem->AddRigidBody(entity, coord, collider.mass, std::move(shape));
        collider.initialized = true;

        std::cout << "ColliderSystem: created shape for entity " << entity << "\n";
    }
}

std::unique_ptr<btCollisionShape> ColliderSystem::CreateShape(const Collider& collider)
{
    switch (collider.shapeType)
    {
    case ColliderShapeType::Sphere:
        return std::make_unique<btSphereShape>(collider.radius);

    case ColliderShapeType::Capsule:
        // btCapsuleShape : rayon + hauteur du cylindre interne (axe Y)
        return std::make_unique<btCapsuleShape>(collider.radius, collider.height);

    case ColliderShapeType::Box:
    default:
        return std::make_unique<btBoxShape>(btVector3(
            collider.halfExtents.x,
            collider.halfExtents.y,
            collider.halfExtents.z
        ));
    }
}