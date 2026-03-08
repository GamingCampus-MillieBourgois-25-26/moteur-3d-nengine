#pragma once
/**
 * @file ECS_Component.h
 * @brief ComponentManager – maps C++ types to IDs and owns all ComponentArray<T>.
 * @ingroup ECS
 */

#include <memory>
#include <cstdint>
#include <cassert>
#include <typeindex>
#include "Engine/ECS/ECS_ComponentArray.h"
#include "Engine/ScriptAPI.h"

/**
 * @brief Central registry for all component types and their data arrays.
 *
 * Responsibilities:
 *  - Assign a unique ComponentType ID to each C++ component type on registration.
 *  - Maintain one ComponentArray<T> per registered type.
 *  - Provide Add / Remove / Get operations forwarded to the appropriate array.
 *  - Propagate entity-destruction events to every registered array.
 *
 * Usage pattern (via Coordinator):
 * @code
 *  componentManager.RegisterComponent<Transform>();
 *  componentManager.AddComponent<Transform>(entity, myTransform);
 *  Transform& t = componentManager.GetComponent<Transform>(entity);
 * @endcode
 */
class ComponentManager
{
private:
    /// @brief Maps a C++ type_index to its unique ComponentType ID.
    std::unordered_map<std::type_index, ComponentType> mComponentTypes;

    /// @brief Stores a ComponentArray<T> for each registered type, type-erased.
    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> mComponentArrays;

    /// @brief Next available component type ID.
    ComponentType mNextComponentType = 0;

public:
    /**
     * @brief Notifies all component arrays that @p entity has been destroyed.
     *
     * Each array removes the entity's component if it owns one.
     * @param entity  The destroyed entity.
     */
    void EntityDestroyed(Entity entity);

    /**
     * @brief Registers a new component type T and allocates its storage array.
     *
     * Must be called once per type before any AddComponent<T>() call.
     * Asserts if T was already registered or MAX_COMPONENTS is exceeded.
     *
     * @tparam T  The component type to register.
     */
    template<typename T>
    void RegisterComponent()
    {
        std::type_index typeName(typeid(T));
        assert(mComponentTypes.find(typeName) == mComponentTypes.end() &&
               "Component type already registered.");
        mComponentTypes[typeName] = mNextComponentType;
        mComponentArrays[typeName] = std::make_unique<ComponentArray<T>>();
        ++mNextComponentType;
        assert(mNextComponentType <= MAX_COMPONENTS &&
               "Exceeded maximum number of components.");
    }

    /**
     * @brief Adds component @p component to @p entity.
     * @tparam T      Component type (must be registered).
     * @param entity  Target entity.
     * @param component  Value to store.
     */
    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        std::type_index typeName(typeid(T));
        auto* arr = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
        assert(arr && "Component not registered.");
        arr->InsertData(entity, component);
    }

    /**
     * @brief Removes the T component from @p entity.
     * @tparam T      Component type (must be registered).
     * @param entity  Target entity.
     */
    template<typename T>
    void RemoveComponent(Entity entity)
    {
        std::type_index typeName(typeid(T));
        auto* arr = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
        assert(arr && "Component not registered.");
        arr->RemoveData(entity);
    }

    /**
     * @brief Returns a reference to the T component owned by @p entity.
     * @tparam T      Component type (must be registered).
     * @param entity  Target entity (must own a T component).
     */
    template<typename T>
    T& GetComponent(Entity entity)
    {
        std::type_index typeName(typeid(T));
        auto* arr = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
        assert(arr && "Component not registered.");
        return arr->GetData(entity);
    }

    /**
     * @brief Returns the unique ID assigned to component type T.
     * @tparam T  Component type (must be registered).
     */
    template<typename T>
    ComponentType GetComponentType()
    {
        std::type_index typeName(typeid(T));
        assert(mComponentTypes.find(typeName) != mComponentTypes.end() &&
               "Component not registered.");
        return mComponentTypes[typeName];
    }
};
