#pragma once
/**
 * @file Velocity.h
 * @brief Component storing the linear velocity of an entity.
 * @ingroup Components
 */

#include "MathsLib/Vector3.h"
#include "MathsLib/Quaternion.h"

/**
 * @brief Represents the linear velocity of an entity (metres per second).
 *
 * The MovementSystem reads this component each frame to integrate
 * the entity's Transform::position.
 *
 * @note Angular velocity is handled separately via
 *       MovementSystem::rotationSpeedX/Y/Z.
 */
struct Velocity
{
    MathsLib::Vector3<float> velocity; ///< Linear velocity vector (m/s).
};
