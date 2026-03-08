#pragma once
/**
 * @file ECS_IComponentArray.h
 * @brief Type-erased base interface for all ComponentArray<T> specialisations.
 * @ingroup ECS
 */

#include "Engine/ECS/ECS_Types.h"

/**
 * @brief Abstract interface shared by all ComponentArray<T> instances.
 *
 * The ComponentManager stores component arrays in a polymorphic container
 * (unordered_map<type_index, unique_ptr<IComponentArray>>).
 * This interface allows the manager to call EntityDestroyed() on every
 * array without knowing the concrete component type T.
 */
class IComponentArray
{
public:
    virtual ~IComponentArray() = default;

    /**
     * @brief Removes the component belonging to @p entity, if it exists.
     *
     * Called automatically by the Coordinator when an entity is destroyed,
     * ensuring no stale data remains in any component array.
     *
     * @param entity  The entity whose component should be removed.
     */
    virtual void EntityDestroyed(Entity entity) = 0;
};
