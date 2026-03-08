#pragma once
/**
 * @file Trigger.h
 * @brief Component that turns a collider into a non-physical detection zone.
 * @ingroup Components
 */

#include "Engine/ECS/ECS_Types.h"
#include <functional>
#include <unordered_set>

/**
 * @brief Marks a collider as a trigger volume.
 *
 * A trigger does not generate physics responses (no forces, no impulses).
 * Instead, the TriggerSystem monitors overlap events and fires user-supplied
 * callbacks:
 *  - **onEnter** – called the first frame another entity enters the volume.
 *  - **onStay**  – called every subsequent frame while the entity remains inside.
 *  - **onExit**  – called the first frame the entity leaves the volume.
 *
 * The TriggerSystem configures the underlying btRigidBody with the
 * CF_NO_CONTACT_RESPONSE flag and uses the btDispatcher to detect pairs.
 *
 * @note The entity must also own a Collider and a Transform for the
 *       trigger volume to be created.
 */
struct Trigger
{
    Trigger() = default;

    /// @brief Fired when @p other enters this trigger for the first time.
    std::function<void(Entity /*self*/, Entity /*other*/)> onEnter = nullptr;

    /// @brief Fired every frame while @p other remains inside this trigger.
    std::function<void(Entity /*self*/, Entity /*other*/)> onStay  = nullptr;

    /// @brief Fired when @p other exits this trigger.
    std::function<void(Entity /*self*/, Entity /*other*/)> onExit  = nullptr;

    /// @brief Internal set of entities currently overlapping (managed by TriggerSystem).
    std::unordered_set<Entity> currentOverlaps;

    bool initialized = false; ///< Set by TriggerSystem once the ghost configuration is applied.
};
