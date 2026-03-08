#pragma once
/**
 * @file PhysicsIncluder.h
 * @brief Convenience header that aggregates all physics-related system includes.
 * @ingroup Engine
 *
 * Including this file provides access to every ECS physics and render system,
 * and defines the PhysicCustom aggregator struct used by Application and Scene.
 */

#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Systems/PhysicsBodySystem.h"
#include "ECS/Systems/ColliderSystem.h"
#include "ECS/Systems/TriggerSystem.h"
#include "ECS/Systems/ForceSystem.h"
#include "ECS/Systems/JointSystem.h"
#include "ECS/Systems/CharacterControllerSystem.h"

/**
 * @brief Groups all physics-related ECS systems into a single struct.
 *
 * Owned by Application and Scene.  Provides a convenient way to pass the
 * entire physics pipeline around without coupling callers to individual
 * system types.
 *
 * All shared_ptr members are null-initialised; the owning class is
 * responsible for creating and linking the systems at startup.
 */
struct PhysicCustom
{
    std::shared_ptr<PhysicsBodySystem>          physicsBodySystem;          ///< Core Bullet world and rigid bodies.
    std::shared_ptr<ColliderSystem>             colliderSystem;             ///< Creates collision shapes.
    std::shared_ptr<TriggerSystem>              triggerSystem;              ///< Overlap event detection.
    std::shared_ptr<ForceSystem>                forceSystem;                ///< Applies forces/impulses.
    std::shared_ptr<JointSystem>                jointSystem;                ///< Manages constraints.
    std::shared_ptr<CharacterControllerSystem>  characterControllerSystem;  ///< Kinematic character movement.
};
