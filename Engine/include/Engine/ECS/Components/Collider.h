#pragma once
/**
 * @file Collider.h
 * @brief Component describing the collision shape of an entity.
 * @ingroup Components
 */

#include "MathsLib/Vector3.h"

/**
 * @brief Supported primitive collision shapes.
 */
enum class ColliderShapeType
{
    Box,     ///< Axis-aligned box, sized by halfExtents.
    Sphere,  ///< Sphere, sized by radius.
    Capsule  ///< Capsule (cylinder + hemispheres), sized by radius and height.
};

/**
 * @brief Defines the Bullet collision shape attached to an entity.
 *
 * The ColliderSystem reads this component and creates the corresponding
 * btCollisionShape, then delegates rigid-body creation to the
 * PhysicsBodySystem.
 *
 * Shape selection:
 *  - **Box**     → uses @p halfExtents.
 *  - **Sphere**  → uses @p radius.
 *  - **Capsule** → uses @p radius and @p height.
 *
 * @note Set @p mass = 0 to create a static (immovable) rigid body.
 */
struct Collider
{
    Collider() = default;

    ColliderShapeType          shapeType   = ColliderShapeType::Box;     ///< Collision primitive.
    MathsLib::Vector3<float>   halfExtents = {0.5f, 0.5f, 0.5f};        ///< Box half-extents (metres).
    float                      radius      = 0.5f;                       ///< Sphere / capsule radius.
    float                      height      = 1.0f;                       ///< Capsule cylinder height.
    float                      mass        = 1.0f;                       ///< Rigid-body mass (0 = static).
    bool                       initialized = false;                      ///< Set by ColliderSystem after creation.
};
