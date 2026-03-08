#pragma once
/**
 * @file PhysicsBodySystem.h
 * @brief Core physics system – owns the Bullet world and all rigid bodies.
 * @ingroup Systems
 */

#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <LinearMath/btVector3.h>
#include <memory>
#include <unordered_map>

/**
 * @brief Owns the Bullet dynamics world and manages all btRigidBody instances.
 *
 * Required components: **Transform**
 *
 * Responsibilities:
 *  - Initialise the full Bullet pipeline (broadphase, dispatcher, solver, world).
 *  - Create and store btRigidBody objects on behalf of ColliderSystem.
 *  - Step the simulation every frame (stepSimulation).
 *  - Write the resulting transform back to each entity's Transform component.
 *  - Provide access to the raw Bullet objects for other systems (ForceSystem,
 *    JointSystem, TriggerSystem, CharacterControllerSystem).
 *
 * All other physics sub-systems depend on PhysicsBodySystem being
 * initialised first via Init().
 */
class PhysicsBodySystem : public System
{
public:
    /// @brief Initialises the Bullet dynamics world (call once at scene startup).
    void Init();

    /**
     * @brief Creates a rigid body for @p entity with the given @p shape and @p mass.
     *
     * Called by ColliderSystem after it has built the btCollisionShape.
     *
     * @param entity    Owning entity (must have a Transform component).
     * @param coord     ECS coordinator for Transform access.
     * @param mass      Body mass (0 = static / kinematic).
     * @param shape     Collision shape (ownership transferred to this system).
     */
    void AddRigidBody(Entity entity, Coordinator& coord, float mass,
                      std::unique_ptr<btCollisionShape> shape);

    /**
     * @brief Removes and destroys the rigid body associated with @p entity.
     * @param entity  Entity whose body should be removed.
     */
    void RemoveRigidBody(Entity entity);

    /**
     * @brief Steps the physics simulation and synchronises Transform components.
     * @param coord  ECS coordinator for Transform write-back.
     * @param dt     Delta-time in seconds.
     */
    void Update(Coordinator& coord, float dt);

    /// @brief Destroys the Bullet world and all rigid bodies.
    void Shutdown();

    /**
     * @brief Returns true if @p entity already has an associated rigid body.
     */
    bool HasRigidBody(Entity entity) const;

    /**
     * @brief Returns the btRigidBody pointer for @p entity, or nullptr if absent.
     */
    btRigidBody* GetRigidBody(Entity entity) const;

    /**
     * @brief Returns a raw pointer to the Bullet dynamics world, or nullptr if
     *        Init() has not been called.
     */
    btDynamicsWorld* GetDynamicsWorld() const;

private:
    std::unique_ptr<btDefaultCollisionConfiguration>     m_collisionConfig; ///< Bullet collision configuration.
    std::unique_ptr<btCollisionDispatcher>               m_dispatcher;      ///< Collision dispatcher.
    std::unique_ptr<btDbvtBroadphase>                    m_broadphase;      ///< Broadphase algorithm.
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;          ///< Constraint solver.
    std::unique_ptr<btDiscreteDynamicsWorld>             m_dynamicsWorld;   ///< Main Bullet world.

    /// @brief Per-entity rigid-body bookkeeping.
    struct BodyData
    {
        std::unique_ptr<btRigidBody>          rigidBody;   ///< Bullet rigid body.
        std::unique_ptr<btCollisionShape>     shape;       ///< Collision shape (owned here).
        std::unique_ptr<btDefaultMotionState> motionState; ///< Synchronises transform to Bullet.
    };

    std::unordered_map<Entity, BodyData> m_bodies; ///< All active rigid bodies, keyed by entity.
};
