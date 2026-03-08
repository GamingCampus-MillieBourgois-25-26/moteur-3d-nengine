#pragma once
/**
 * @file CharacterControllerSystem.h
 * @brief System managing kinematic character controllers via Bullet Physics.
 * @ingroup Systems
 */

#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/CharacterController.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Systems/PhysicsBodySystem.h"
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <memory>
#include <unordered_map>

/**
 * @brief Creates and drives btKinematicCharacterController instances.
 *
 * Required components: **Transform** + **CharacterController**
 *
 * For each entity with an uninitialised CharacterController the system:
 *  1. Creates a btCapsuleShape sized by capsuleRadius / capsuleHeight.
 *  2. Creates a btPairCachingGhostObject at the entity's Transform position.
 *  3. Creates a btKinematicCharacterController and registers it with the world.
 *
 * Every frame Update():
 *  - Feeds CharacterController::walkDirection to the controller.
 *  - Triggers a jump if CharacterController::jump is true.
 *  - Updates CharacterController::onGround.
 *  - Writes the new position back to the entity's Transform component.
 *
 * PhysicsBodySystem remains the sole owner of the Bullet dynamics world.
 */
class CharacterControllerSystem : public System
{
public:
    /**
     * @brief Connects this system to the physics backend.
     * @param physicsSystem  Shared pointer to the initialised PhysicsBodySystem.
     */
    void Init(std::shared_ptr<PhysicsBodySystem> physicsSystem);

    /**
     * @brief Creates controllers for new entities and drives existing ones.
     * @param coord  ECS coordinator for component access.
     * @param dt     Delta-time in seconds.
     */
    void Update(Coordinator& coord, float dt);

    /**
     * @brief Removes the character controller associated with @p entity.
     * @param entity  Entity to clean up.
     */
    void RemoveController(Entity entity);

    /// @brief Destroys all character controllers and releases Bullet resources.
    void Shutdown();

private:
    /**
     * @brief Allocates Bullet objects and registers the controller for @p entity.
     * @param entity  Entity with an uninitialised CharacterController component.
     * @param coord   ECS coordinator.
     */
    void CreateController(Entity entity, Coordinator& coord);

    std::shared_ptr<PhysicsBodySystem> m_physicsSystem; ///< Physics backend.

    /// @brief Bullet objects owned per-entity.
    struct ControllerData
    {
        std::unique_ptr<btKinematicCharacterController> controller;  ///< Kinematic controller.
        std::unique_ptr<btPairCachingGhostObject>       ghostObject; ///< Ghost collision object.
        std::unique_ptr<btCapsuleShape>                 shape;       ///< Capsule collision shape.
    };

    std::unordered_map<Entity, ControllerData> m_controllers; ///< All active controllers.
};
