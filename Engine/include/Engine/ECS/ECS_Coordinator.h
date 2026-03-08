#pragma once
/**
 * @file ECS_Coordinator.h
 * @brief Unified facade for the entire Entity-Component-System.
 * @ingroup ECS
 */

#include <memory>
#include <cstdint>
#include <bitset>
#include <cassert>
#include <typeindex>
#include <unordered_map>
#include <set>
#include "Engine/ECS/ECS_Types.h"
#include "Engine/ECS/ECS_Entity.h"
#include "Engine/ECS/ECS_Component.h"
#include "Engine/ECS/ECS_ComponentArray.h"
#include "Engine/ECS/ECS_SystemManager.h"

/**
 * @brief Central facade that connects entities, components and systems.
 *
 * The Coordinator owns three managers internally:
 *  - **EntityManager**    – creates/destroys entities and stores their signatures.
 *  - **ComponentManager** – maps C++ types to IDs and stores component data arrays.
 *  - **SystemManager**    – registers systems and keeps their entity sets updated.
 *
 * All engine code should go through the Coordinator rather than the managers
 * directly.  A typical usage pattern:
 *
 * @code
 *  Coordinator coord;
 *  coord.Init();
 *  coord.RegisterComponent<Transform>();
 *  Entity e = coord.CreateEntity();
 *  coord.AddComponent<Transform>(e, Transform{});
 *  auto sys = coord.RegisterSystem<MovementSystem>();
 *  Signature sig;
 *  sig.set(coord.GetComponentType<Transform>());
 *  coord.SetSystemSignature<MovementSystem>(sig);
 * @endcode
 */
class Coordinator
{
public:
    std::unique_ptr<ComponentManager> mComponentManager; ///< Manages component storage.
    std::unique_ptr<EntityManager>    mEntityManager;    ///< Manages entity IDs & signatures.
    std::unique_ptr<SystemManager>    mSystemManager;    ///< Manages system registration.

    /**
     * @brief Initialises all three internal managers.
     * Must be called before any other Coordinator method.
     */
    void Init();

    // ── Entity API ───────────────────────────────────────────────────────────

    /**
     * @brief Creates a new entity and returns its ID.
     * @return Newly created entity ID.
     */
    Entity CreateEntity();

    /**
     * @brief Destroys @p entity and removes all its components from every system.
     * @param entity  Entity to destroy.
     */
    void DestroyEntity(Entity entity);

    // ── Component API ────────────────────────────────────────────────────────

    /**
     * @brief Registers component type T so it can be used with AddComponent.
     * @tparam T  Component type to register.
     */
    template<typename T>
    void RegisterComponent()
    {
        mComponentManager->RegisterComponent<T>();
    }

    /**
     * @brief Adds a component of type T to @p entity.
     *
     * Updates the entity's signature and notifies all systems.
     *
     * @tparam T      Component type.
     * @param entity  Target entity.
     * @param component  Component value to attach.
     */
    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        mComponentManager->AddComponent<T>(entity, component);
        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>());
        mEntityManager->SetSignature(entity, signature);
        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    /**
     * @brief Removes the T component from @p entity.
     *
     * Updates the entity's signature and notifies all systems.
     *
     * @tparam T      Component type.
     * @param entity  Target entity.
     */
    template<typename T>
    void RemoveComponent(Entity entity)
    {
        mComponentManager->RemoveComponent<T>(entity);
        auto signature = mEntityManager->GetSignature(entity);
        signature.reset(mComponentManager->GetComponentType<T>());
        mEntityManager->SetSignature(entity, signature);
        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    /**
     * @brief Returns a reference to the T component of @p entity.
     * @tparam T      Component type.
     * @param entity  Target entity (must own a T component).
     */
    template<typename T>
    T& GetComponent(Entity entity)
    {
        return mComponentManager->GetComponent<T>(entity);
    }

    /**
     * @brief Returns the unique ID assigned to component type T.
     * @tparam T  Registered component type.
     */
    template<typename T>
    ComponentType GetComponentType()
    {
        return mComponentManager->GetComponentType<T>();
    }

    // ── System API ───────────────────────────────────────────────────────────

    /**
     * @brief Registers a new system of type T.
     * @tparam T  System type derived from System.
     * @return    Shared pointer to the created system.
     */
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return mSystemManager->RegisterSystem<T>();
    }

    /**
     * @brief Sets the component signature required by system T.
     *
     * Entities lacking any of the required components are excluded
     * from the system's entity set.
     *
     * @tparam T        System type.
     * @param signature Bitmask of required component types.
     */
    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        mSystemManager->SetSignature<T>(signature);
    }
};
