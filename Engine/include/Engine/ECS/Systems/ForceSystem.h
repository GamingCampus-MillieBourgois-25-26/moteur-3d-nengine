#pragma once
/**
 * @file ForceSystem.h
 * @brief System that applies Force components to Bullet rigid bodies.
 * @ingroup Systems
 */

#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Force.h"
#include "Engine/ECS/Systems/PhysicsBodySystem.h"
#include <memory>

/**
 * @brief Forwards linear forces and torques from Force components to btRigidBody.
 *
 * Required components: **Force** (Force::active == true)
 *
 * Each physics tick:
 *  - **ForceMode::Force**   → calls btRigidBody::applyCentralForce() / applyTorque()
 *    every frame (force remains active).
 *  - **ForceMode::Impulse** → calls btRigidBody::applyCentralImpulse() / applyTorqueImpulse()
 *    once, then resets the Force component and clears the active flag.
 *
 * PhysicsBodySystem owns the btRigidBody instances; this system only reads them.
 */
class ForceSystem : public System
{
public:
    /**
     * @brief Connects this system to the physics backend.
     * @param physicsSystem  Shared pointer to the already-initialised PhysicsBodySystem.
     */
    void Init(std::shared_ptr<PhysicsBodySystem> physicsSystem);

    /**
     * @brief Applies all active Force components to their rigid bodies.
     * @param coord  ECS coordinator for component access.
     */
    void Update(Coordinator& coord);

private:
    std::shared_ptr<PhysicsBodySystem> m_physicsSystem; ///< Physics backend (non-owning reference).
};
