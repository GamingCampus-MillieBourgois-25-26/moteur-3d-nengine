#pragma once
/**
 * @file ECS_SystemManager.h
 * @brief Manages system registration and entity-signature routing.
 * @ingroup ECS
 */

#include <iostream>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <cassert>
#include "Engine/ECS/ECS_Types.h"
#include "Engine/ECS/ECS_System.h"

/**
 * @brief Registers systems, assigns component signatures, and keeps
 *        each system's entity set up-to-date.
 *
 * Role:
 *  - Register systems (MovementSystem, RenderSystem, …).
 *  - Associate a required component Signature with each system.
 *  - When an entity's signature changes, add or remove it from each
 *    system whose required signature is a subset of the entity's signature.
 *
 * This is the "wiring" layer that connects entities to the systems
 * that should process them.
 */
class SystemManager
{
private:
    /// @brief Required component bitmask for each registered system type.
    std::unordered_map<std::type_index, Signature> mSignatures;

    /// @brief Shared pointers to every registered system instance.
    std::unordered_map<std::type_index, std::shared_ptr<System>> mSystems;

public:
    SystemManager()  = default;
    ~SystemManager() = default;

    /**
     * @brief Registers a new system of type T and returns a shared_ptr to it.
     *
     * Each system type may be registered only once (asserted).
     * @tparam T  System type derived from System.
     * @return    Shared pointer to the newly created system instance.
     */
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        std::type_index typeName = typeid(T);
        assert(mSystems.find(typeName) == mSystems.end() &&
               "System already registered.");
        auto system = std::make_shared<T>();
        mSystems[typeName] = system;
        return system;
    }

    /**
     * @brief Sets the component signature required by system T.
     *
     * An entity is added to T::mEntities only when its own signature
     * contains all bits set in @p signature.
     *
     * @tparam T        System type (must be registered first).
     * @param signature Bitmask of required component types.
     */
    template<typename T>
    void SetSignature(Signature signature)
    {
        std::type_index typeName = typeid(T);
        assert(mSystems.find(typeName) != mSystems.end() &&
               "System not registered.");
        mSignatures[typeName] = signature;
    }

    /**
     * @brief Removes @p entity from all systems (called when the entity is destroyed).
     * @param entity  The destroyed entity.
     */
    void EntityDestroyed(Entity entity);

    /**
     * @brief Updates every system's entity set when @p entity gains or loses a component.
     *
     * For each registered system, checks whether @p entitySignature satisfies
     * the system's required signature and adds/removes the entity accordingly.
     *
     * @param entity           The entity whose signature changed.
     * @param entitySignature  The entity's updated component bitmask.
     */
    void EntitySignatureChanged(Entity entity, Signature entitySignature);
};
