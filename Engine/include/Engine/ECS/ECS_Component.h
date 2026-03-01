#pragma once
#include <memory> 
#include <cstdint> 
#include <cassert>
#include <typeindex>
#include "Engine/ECS/ECS_ComponentArray.h"

/*
    ComponentManager
    ----------------
    - Associe un type C++ (Transform, Velocity...) à un ID de composant
    - Stocke un ComponentArray<T> pour chaque type
    - Ajoute / retire / récupère un composant pour une entité
    - Nettoie les composants quand une entité est détruite
*/

class ComponentManager 
{

private:
    // Associe un type C++ à un ID de composant
    std::unordered_map<std::type_index, ComponentType> mComponentTypes;

    // Stocke un ComponentArray<T> pour chaque type
    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> mComponentArrays;

    // Prochain ID de composant disponible
    ComponentType mNextComponentType = 0;

public:

    // Supprime les composants d'une entité dans tous les ComponentArray
    void EntityDestroyed(Entity entity);
    
    // Enregistre un nouveau type de composant
    template<typename T>
    void RegisterComponent()
    {
        std::type_index typeName(typeid(T));

        // Vérifie si le composant est déjà enregistré
        assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Component type already registered.");

        // Assigne un ID unique
        mComponentTypes[typeName] = mNextComponentType;

        // Crée et stocke un ComponentArray<T>
        mComponentArrays[typeName] = std::make_unique<ComponentArray<T>>();

        ++mNextComponentType;
        assert(mNextComponentType <= MAX_COMPONENTS && "Exceeded maximum number of components.");
    }

    // Ajoute un composant à une entité
    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        std::type_index typeName(typeid(T));

        auto componentArray = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
        assert(componentArray && "Component not registered.");

        componentArray->InsertData(entity, component);
    }

    // Supprime un composant d'une entité
    template<typename T>
    void RemoveComponent(Entity entity) {
        std::type_index typeName(typeid(T));

        auto componentArray = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
        assert(componentArray && "Component not registered.");

        componentArray->RemoveData(entity);
    }

    // Récupère une référence au composant d'une entité
    template<typename T>
    T& GetComponent(Entity entity)
    {
        std::type_index typeName(typeid(T));

        //auto componentArray = std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
        auto componentArray = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
        assert(componentArray && "Component not registered.");

        return componentArray->GetData(entity);
    }

    // Retourne l'ID du type de composant T
    template<typename T>
    ComponentType GetComponentType()
    {
        std::type_index typeName(typeid(T));
        assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered.");

        return mComponentTypes[typeName];
    }
};