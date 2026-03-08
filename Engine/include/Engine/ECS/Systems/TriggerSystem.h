#pragma once
/**
 * @file TriggerSystem.h
 * @brief System that detects overlap events for trigger volumes.
 * @ingroup Systems
 */

#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Trigger.h"
#include "Engine/ECS/Systems/PhysicsBodySystem.h"
#include "Engine/ECS/Components/Collider.h"
#include "Engine/ECS/Components/Transform.h"
#include <btBulletDynamicsCommon.h>
#include <unordered_set>
#include <unordered_map>
#include <memory>

/**
 * @brief Detects enter / stay / exit overlap events for Trigger volumes.
 *
 * Required components: **Transform** + **Collider** + **Trigger**
 *
 * The system:
 *  1. Configures new Trigger entities by setting CF_NO_CONTACT_RESPONSE
 *     on their btRigidBody (so they do not generate physics responses).
 *  2. Every frame, iterates the btDispatcher's contact manifolds to find
 *     all pairs that involve at least one trigger entity.
 *  3. Compares the new overlap set against the previous frame's set stored
 *     in Trigger::currentOverlaps to classify events.
 *  4. Calls the appropriate callback (onEnter, onStay, onExit).
 *
 * Internally, a reverse map from btCollisionObject* to Entity is maintained
 * for efficient pair resolution.
 */
class TriggerSystem : public System
{
public:
    /**
     * @brief Connects this system to the physics backend.
     * @param physicsSystem  Shared pointer to the initialised PhysicsBodySystem.
     */
    void Init(std::shared_ptr<PhysicsBodySystem> physicsSystem);

    /**
     * @brief Detects overlap changes and fires trigger callbacks.
     * @param coord  ECS coordinator for component access.
     */
    void Update(Coordinator& coord);

private:
    /**
     * @brief Marks a rigid body as a ghost (no contact response).
     * @param entity  Entity with Trigger + Collider components.
     * @param coord   ECS coordinator.
     */
    void ConfigureAsTrigger(Entity entity, Coordinator& coord);

    /**
     * @brief Resolves a Bullet collision object back to its owning Entity.
     * @param body  The btCollisionObject to look up.
     * @return The owning Entity, or an invalid ID if not found.
     */
    Entity FindEntityFromBody(const btCollisionObject* body) const;

    std::shared_ptr<PhysicsBodySystem> m_physicsSystem; ///< Physics backend.
};
