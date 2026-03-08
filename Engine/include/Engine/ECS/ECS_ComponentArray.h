#pragma once
/**
 * @file ECS_ComponentArray.h
 * @brief Cache-friendly, O(1) dense component storage.
 * @ingroup ECS
 */

#include <array>
#include <unordered_map>
#include <cassert>
#include "Engine/ECS/ECS_Types.h"
#include "Engine/ECS/ECS_IComponentArray.h"

/**
 * @brief Contiguous array storing all components of type T.
 *
 * @tparam T  Component type (e.g. Transform, Velocity, MeshRenderer).
 *
 * Design goals:
 *  - **Cache-friendly** – all T values are stored in a single flat array.
 *  - **O(1) insertion** – new components are appended at the end.
 *  - **O(1) removal**   – the removed slot is back-filled with the last element.
 *  - **O(1) lookup**    – entity → array index via an unordered_map.
 *
 * Internal bookkeeping maps:
 *  - mEntityToIndex : Entity → array index.
 *  - mIndexToEntity : array index → Entity (needed for the swap-remove).
 */
template<typename T>
class ComponentArray : public IComponentArray
{
public:
    std::array<T, MAX_ENTITIES>          mComponentArray; ///< Flat component storage.
    std::unordered_map<Entity, size_t>   mEntityToIndex;  ///< Entity  → storage index.
    std::unordered_map<size_t, Entity>   mIndexToEntity;  ///< Index   → entity (for swap-remove).
    size_t                               mSize = 0;       ///< Number of active components.

    /**
     * @brief Inserts a component for @p entity.
     * @param entity     The owning entity (must not already own a T component).
     * @param component  Component value to store.
     */
    void InsertData(Entity entity, T component);

    /**
     * @brief Removes the component of @p entity using a swap-and-pop.
     *
     * The last element is moved into the freed slot to keep the array dense.
     * @param entity  The entity whose component should be removed.
     */
    void RemoveData(Entity entity);

    /**
     * @brief Returns a reference to the component owned by @p entity.
     * @param entity  Must own a T component (asserted in debug).
     */
    T& GetData(Entity entity);

    /**
     * @brief IComponentArray override – removes the component if the entity owns one.
     * @param entity  The destroyed entity.
     */
    void EntityDestroyed(Entity entity) override;
};
