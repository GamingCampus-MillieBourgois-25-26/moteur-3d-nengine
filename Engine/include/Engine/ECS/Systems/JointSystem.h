#pragma once
/**
 * @file JointSystem.h
 * @brief System that creates and manages Bullet constraints between entities.
 * @ingroup Systems
 */

#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Joint.h"
#include "Engine/ECS/Systems/PhysicsBodySystem.h"
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h>
#include <BulletDynamics/ConstraintSolver/btHingeConstraint.h>
#include <BulletDynamics/ConstraintSolver/btFixedConstraint.h>
#include <memory>
#include <unordered_map>

/**
 * @brief Creates btTypedConstraint objects from Joint components.
 *
 * Required components: **Joint** (Joint::initialized == false)
 *
 * The system defers to PhysicsBodySystem for both the Bullet world and the
 * btRigidBody pointers.  Once a constraint is built it is registered with
 * the world and tracked internally for later cleanup.
 *
 * Supported constraint types:
 *  - **Point2Point** → btPoint2PointConstraint (ball-and-socket).
 *  - **Hinge**       → btHingeConstraint (single-axis pivot with optional limits).
 *  - **Fixed**       → btFixedConstraint (rigid link, no degrees of freedom).
 */
class JointSystem : public System
{
public:
    /**
     * @brief Connects this system to the physics backend.
     * @param physicsSystem  Shared pointer to the initialised PhysicsBodySystem.
     */
    void Init(std::shared_ptr<PhysicsBodySystem> physicsSystem);

    /**
     * @brief Creates constraints for all uninitialised Joint components.
     * @param coord  ECS coordinator for component access.
     */
    void Update(Coordinator& coord);

    /**
     * @brief Removes and destroys the constraint associated with @p entity.
     * @param entity  Entity whose Joint should be removed from the world.
     */
    void RemoveJoint(Entity entity);

    /// @brief Removes all active constraints and frees their resources.
    void Shutdown();

private:
    /**
     * @brief Factory: builds the correct btTypedConstraint from a Joint descriptor.
     * @param joint  Source Joint component.
     * @param bodyA  Rigid body of the source entity.
     * @param bodyB  Rigid body of the target entity.
     * @return Owning pointer to the new constraint.
     */
    std::unique_ptr<btTypedConstraint> CreateConstraint(
        const Joint& joint,
        btRigidBody* bodyA,
        btRigidBody* bodyB);

    std::shared_ptr<PhysicsBodySystem> m_physicsSystem; ///< Physics backend.

    /// @brief Bookkeeping for one active constraint.
    struct JointData
    {
        std::unique_ptr<btTypedConstraint> constraint; ///< Owned Bullet constraint.
    };

    std::unordered_map<Entity, JointData> m_joints; ///< Active constraints keyed by entity.
};
