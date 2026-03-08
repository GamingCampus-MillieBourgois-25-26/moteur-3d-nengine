#pragma once
/**
 * @file Joint.h
 * @brief Component describing a physical constraint between two entities.
 * @ingroup Components
 */

#include "MathsLib/Vector3.h"
#include "Engine/ECS/ECS_Types.h"

/**
 * @brief Available Bullet constraint types.
 */
enum class JointType
{
    Point2Point, ///< Ball-and-socket (free rotation around a shared pivot).
    Hinge,       ///< Pivot joint (rotation on a single axis with optional limits).
    Fixed        ///< Rigid link (no relative movement between the two bodies).
};

/**
 * @brief Constrains the motion between two rigid-body entities.
 *
 * The JointSystem reads this component, retrieves the two btRigidBody
 * pointers from the PhysicsBodySystem, creates the appropriate
 * btTypedConstraint, and registers it with the Bullet dynamics world.
 *
 * Hinge limits (lowerLimit / upperLimit) are in radians.
 * Setting @p breakingForce = 0 makes the joint unbreakable.
 *
 * @note Both entities must own a Collider so their rigid bodies exist
 *       before the joint is initialised.
 */
struct Joint
{
    Joint() = default;

    JointType type         = JointType::Point2Point; ///< Constraint variety.
    Entity    targetEntity = 0;                       ///< The other end of the constraint.

    MathsLib::Vector3<float> pivotA = {0.f, 0.f, 0.f}; ///< Local pivot on this entity.
    MathsLib::Vector3<float> pivotB = {0.f, 0.f, 0.f}; ///< Local pivot on the target entity.

    /// @brief Hinge rotation axis in local space of this entity (default: Y-up).
    MathsLib::Vector3<float> axisA  = {0.f, 1.f, 0.f};
    /// @brief Hinge rotation axis in local space of the target entity (default: Y-up).
    MathsLib::Vector3<float> axisB  = {0.f, 1.f, 0.f};

    float lowerLimit    = 0.f; ///< Hinge minimum angle (radians).
    float upperLimit    = 0.f; ///< Hinge maximum angle (radians).
    float breakingForce = 0.f; ///< Force threshold that breaks the joint (0 = infinite).

    bool  initialized   = false; ///< Set by JointSystem after the constraint is created.
};
