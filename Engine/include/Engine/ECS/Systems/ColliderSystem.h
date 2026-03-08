#pragma once
/**
 * @file ColliderSystem.h
 * @brief System that creates Bullet collision shapes from Collider components.
 * @ingroup Systems
 */

#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Collider.h"
#include "Engine/ECS/Systems/PhysicsBodySystem.h"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <LinearMath/btVector3.h>
#include <memory>

/**
 * @brief Instantiates Bullet collision shapes and delegates rigid-body creation.
 *
 * Required components: **Transform** + **Collider** (Collider::initialized == false)
 *
 * On each Update() call the system:
 *  1. Iterates entities whose Collider has not yet been initialised.
 *  2. Calls CreateShape() to build the matching btCollisionShape.
 *  3. Passes the shape and mass to PhysicsBodySystem::AddRigidBody().
 *  4. Marks Collider::initialized = true.
 *
 * PhysicsBodySystem remains the sole owner of the Bullet world.
 */
class ColliderSystem : public System
{
public:
    /**
     * @brief Connects this system to the physics backend.
     * @param physicsSystem  Shared pointer to the already-initialised PhysicsBodySystem.
     */
    void Init(std::shared_ptr<PhysicsBodySystem> physicsSystem);

    /**
     * @brief Processes all uninitialised Collider components and creates their shapes.
     * @param coord  ECS coordinator for component access.
     */
    void Update(Coordinator& coord);

private:
    /**
     * @brief Factory function that builds a btCollisionShape from a Collider descriptor.
     * @param collider  Source Collider component.
     * @return Owning pointer to the new btCollisionShape.
     */
    std::unique_ptr<btCollisionShape> CreateShape(const Collider& collider);

    std::shared_ptr<PhysicsBodySystem> m_physicsSystem; ///< Reference to the physics backend.
};
