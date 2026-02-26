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
    template<typename T>
    void RegisterComponent(); // assigne un ID unique (0,1,2), cree un ComponentArray<Transform>

    template<typename T>
    void AddComponent(Entity entity, T component);

    template<typename T>
    T& GetComponent(Entity entity);

    template<typename T>
    ComponentType GetComponentType();

    void EntityDestroyed(Entity entity);

private:
    std::unordered_map<std::type_index, ComponentType> mComponentTypes;
    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> mComponentArrays;

    ComponentType mNextComponentType = 0;
};