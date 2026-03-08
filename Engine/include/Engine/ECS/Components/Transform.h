#pragma once
/**
 * @file Transform.h
 * @brief Component describing the spatial state of an entity.
 * @ingroup Components
 */

#include "MathsLib/Vector3.h"
#include "MathsLib/Quaternion.h"

/**
 * @brief Holds the position, rotation and scale of an entity in world space.
 *
 * Transform is the most fundamental component in the engine.
 * It is required by virtually every other system (Renderer, Physics, Movement).
 *
 * @note The physics simulation writes back to this component every frame
 *       after stepping the Bullet world.
 */
struct Transform
{
    Transform() = default;

    MathsLib::Vector3<float>    position; ///< World-space position (metres).
    MathsLib::Quaternion<float> rotation; ///< Orientation as a unit quaternion.
    MathsLib::Vector3<float>    scale;    ///< Non-uniform scale factor per axis.
};
