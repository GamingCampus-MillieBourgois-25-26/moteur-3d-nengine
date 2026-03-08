#pragma once
/**
 * @file MovementSystem.h
 * @brief System that integrates velocity and angular velocity into Transform.
 * @ingroup Systems
 */

#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Velocity.h"
#include "MathsLib/Quaternion.h"

/**
 * @brief Applies linear (Velocity) and angular motion to entity Transforms.
 *
 * Required components: **Transform** + **Velocity**
 *
 * Each frame Update() integrates:
 *  - Linear:  Transform::position += Velocity::velocity * dt
 *  - Angular: Transform::rotation composed with rotations derived from
 *             rotationSpeedX/Y/Z * dt
 *
 * This system runs on the CPU independently of Bullet Physics.
 * For physics-driven entities, use the physics pipeline instead.
 */
class MovementSystem : public System
{
public:
    float rotationSpeedX = 0.0f; ///< Angular velocity around the X axis (radians/s).
    float rotationSpeedY = 0.0f; ///< Angular velocity around the Y axis (radians/s).
    float rotationSpeedZ = 0.0f; ///< Angular velocity around the Z axis (radians/s).

    /**
     * @brief Integrates all entities' Transform components using their Velocity.
     * @param coord  The ECS coordinator used to access components.
     * @param dt     Delta-time in seconds since the last frame.
     */
    void Update(Coordinator& coord, float dt);
};
