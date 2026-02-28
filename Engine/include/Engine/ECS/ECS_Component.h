#pragma once
#include <memory> 
#include <cstdint> 
#include <cassert>
#include <typeindex>
#include "Engine/ECS/ECS_ComponentArray.h"

// Composant : un composant est un struct simple avec seuelement des valeurs sans qu'elles soient definies (pure)

class ComponentManager // Role : associer un type C++ a un ID de composant, gerer les ComponentArray
{
public:
    /*
    template<typename T>
    void RegisterComponent(); // assigne un ID unique (0,1,2), cree un ComponentArray<Transform>

    template<typename T>
    void AddComponent(Entity entity, T component);

    template<typename T>
    void RemoveComponent(Entity entity);

    template<typename T>
    T& GetComponent(Entity entity);

    template<typename T>
    ComponentType GetComponentType();*/

    void EntityDestroyed(Entity entity);
    

    template<typename T>
    void RegisterComponent()
    {
        std::type_index typeName(typeid(T));

        // Vérifie si le composant est déjà enregistré
        assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Component type already registered.");

        // Assigne un ID unique
        mComponentTypes[typeName] = mNextComponentType;

        // Crée et stocke le ComponentArray<T>
        mComponentArrays[typeName] = std::make_unique<ComponentArray<T>>();

        ++mNextComponentType;
        assert(mNextComponentType <= MAX_COMPONENTS && "Exceeded maximum number of components.");
    }

    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        std::type_index typeName(typeid(T));

        auto componentArray = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
        assert(componentArray && "Component not registered.");

        componentArray->InsertData(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        std::type_index typeName(typeid(T));

        auto componentArray = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
        assert(componentArray && "Component not registered.");

        componentArray->RemoveData(entity);
    }

    template<typename T>
    T& GetComponent(Entity entity)
    {
        std::type_index typeName(typeid(T));

        //auto componentArray = std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
        auto componentArray = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
        assert(componentArray && "Component not registered.");

        return componentArray->GetData(entity);
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        std::type_index typeName(typeid(T));
        assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered.");

        return mComponentTypes[typeName];
    }

private:
    std::unordered_map<std::type_index, ComponentType> mComponentTypes;
    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> mComponentArrays;

    ComponentType mNextComponentType = 0;
};