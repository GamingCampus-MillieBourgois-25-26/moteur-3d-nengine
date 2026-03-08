#pragma once
/**
 * @file Force.h
 * @brief Component describing forces or impulses applied to a rigid body.
 * @ingroup Components
 */

#include "MathsLib/Vector3.h"

/**
 * @brief Selects how a Force component is applied by the ForceSystem.
 */
enum class ForceMode
{
    Force,   ///< Continuous force (Newtons) applied every physics frame.
    Impulse  ///< Instantaneous impulse (Newton·seconds) applied once, then reset.
};

/**
 * @brief Carries force/torque data that will be applied to a Bullet rigid body.
 *
 * The ForceSystem reads this component each physics tick, calls the
 * appropriate btRigidBody functions, and – for Impulse mode – resets
 * the vectors and clears the @p active flag afterwards.
 *
 * @note Only entities that also have a Collider (and therefore a rigid body)
 *       will produce visible physical effects.
 */
struct Force
{
    Force() = default;

    MathsLib::Vector3<float> force  = {0.f, 0.f, 0.f}; ///< Linear force / impulse vector (N or N·s).
    MathsLib::Vector3<float> torque = {0.f, 0.f, 0.f}; ///< Angular torque / impulse vector.
    ForceMode mode   = ForceMode::Force;                ///< How the force is applied.
    bool      active = false;                           ///< True when the component has pending forces.
};
