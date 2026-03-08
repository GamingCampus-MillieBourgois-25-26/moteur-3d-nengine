#pragma once
/**
 * @file ECS_System.h
 * @brief Base class for all ECS systems.
 * @ingroup ECS
 */

#include "Engine/ECS/ECS_Types.h"
#include <set>

/**
 * @brief Abstract base class for every game system (MovementSystem, RenderSystem, …).
 *
 * A System contains **logic only** – it never owns component data.
 * The SystemManager automatically populates mEntities with every entity
 * whose component signature matches the system's registered signature.
 *
 * Derived systems typically override Update(), Render(), or a custom
 * tick method and iterate over mEntities to process matching components.
 */
class System
{
public:
    /**
     * @brief Set of entities whose signature satisfies this system's requirements.
     *
     * Maintained automatically by the SystemManager; systems should treat
     * this as read-only and iterate over it during their update step.
     */
    std::set<Entity> mEntities;
};
