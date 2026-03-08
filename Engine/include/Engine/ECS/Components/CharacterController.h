#pragma once
/**
 * @file CharacterController.h
 * @brief Component for kinematic character movement using Bullet Physics.
 * @ingroup Components
 */

#include "MathsLib/Vector3.h"
#include "Engine/ECS/ECS_Types.h"

/**
 * @brief Defines the physical properties of a playable kinematic character.
 *
 * The CharacterControllerSystem reads this component to create a
 * btKinematicCharacterController backed by a btPairCachingGhostObject
 * and a btCapsuleShape inside the Bullet dynamics world.
 *
 * Each frame the system:
 *  1. Applies @p walkDirection and @p jump to the controller.
 *  2. Synchronises the resulting position back into the entity's Transform.
 *
 * Workflow:
 *  - Set @p walkDirection every frame from gameplay code (input system).
 *  - Set @p jump = true to trigger a jump; the system consumes it.
 *  - Read @p onGround to determine whether the character can jump again.
 */
struct CharacterController
{
    CharacterController() = default;

    // ── Capsule dimensions ────────────────────────────────────────────────

    float capsuleRadius = 0.4f; ///< Capsule collision radius (metres).
    float capsuleHeight = 1.0f; ///< Capsule cylinder height  (metres).

    // ── Movement parameters ───────────────────────────────────────────────

    float walkSpeed     = 5.0f;  ///< Horizontal movement speed (m/s).
    float jumpSpeed     = 6.0f;  ///< Initial vertical velocity on jump (m/s).
    float maxSlopeAngle = 0.78f; ///< Maximum walkable slope (radians, ~45°).
    float stepHeight    = 0.35f; ///< Maximum step the character can climb (metres).
    float gravity       = -9.81f;///< Gravitational acceleration applied to the character (m/s²).

    // ── Runtime state ─────────────────────────────────────────────────────

    /// @brief Desired movement direction this frame (set by gameplay code).
    MathsLib::Vector3<float> walkDirection = {0.f, 0.f, 0.f};

    bool jump        = false; ///< Set true to request a jump; consumed by CharacterControllerSystem.
    bool onGround    = false; ///< True when the character is touching the ground (read-only from gameplay).
    bool initialized = false; ///< Set by CharacterControllerSystem after controller creation.
};
