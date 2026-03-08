#pragma once
/**
 * @file ECS_Entity.h
 * @brief Entity lifecycle management – ID allocation and signature storage.
 * @ingroup ECS
 *
 * ECS architecture overview:
 *  - Entity            → simple integer ID
 *  - Signature         → bitset of owned component types
 *  - EntityManager     → distributes/recycles IDs and stores signatures
 *  - ComponentArray<T> → contiguous storage for one component type
 *  - ComponentManager  → owns all ComponentArrays
 *  - System            → base class for game logic
 *  - SystemManager     → maps systems to their required signatures
 *  - Coordinator       → unified facade
 */

#include <queue>
#include <array>
#include <cstdint>
#include <cassert>
#include "Engine/ECS/ECS_Types.h"

/**
 * @brief Manages entity IDs and their component signatures.
 *
 * Responsibilities:
 *  - Distribute free IDs from an internal queue (0 → MAX_ENTITIES-1).
 *  - Recycle IDs when entities are destroyed.
 *  - Store and retrieve the component signature of every living entity.
 *  - Enforce the MAX_ENTITIES limit via assertions.
 */
class EntityManager
{
private:
    /// @brief Pool of free (recyclable) entity IDs.
    std::queue<Entity> mAvailableEntities;

    /// @brief Per-entity component signature, indexed by entity ID.
    std::array<Signature, MAX_ENTITIES> mSignatures{};

    /// @brief Number of entities currently alive.
    std::uint32_t mLivingEntityCount = 0;

public:
    /**
     * @brief Initialises the manager by filling the ID pool with [0, MAX_ENTITIES).
     */
    EntityManager();

    /**
     * @brief Creates a new entity by taking a free ID from the pool.
     * @return The new entity's ID.
     * @note Asserts if MAX_ENTITIES would be exceeded.
     */
    Entity CreateEntity();

    /**
     * @brief Destroys an entity: resets its signature and returns its ID to the pool.
     * @param entity  The entity to destroy.
     */
    void DestroyEntity(Entity entity);

    /**
     * @brief Stores the component signature for @p entity.
     * @param entity     Target entity.
     * @param signature  New component bitmask.
     */
    void SetSignature(Entity entity, Signature signature);

    /**
     * @brief Retrieves the component signature of @p entity.
     * @param entity  Target entity.
     * @return The entity's current component bitmask.
     */
    Signature GetSignature(Entity entity) const;
};
