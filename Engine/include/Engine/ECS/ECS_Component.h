#pragma once
#include <memory> 
#include <cstdint> 
#include <cassert>
#include <typeindex>
#include "Engine/ECS/ECS_ComponentArray.h"

/**
 * @brief Gestionnaire central des composants ECS.
 *
 * Associe chaque type de composant C++ à un ID unique,
 * stocke un tableau de composants pour chaque type,
 * et fournit les opérations d'ajout, suppression et accès
 * aux composants des entités.
 */
class ComponentManager
{
private:
    /** @brief Associe un type C++ à un ID de composant. */
    std::unordered_map<std::type_index, ComponentType> mComponentTypes;

    /** @brief Stocke un ComponentArray<T> pour chaque type de composant. */
    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> mComponentArrays;

    /** @brief Prochain ID de composant disponible. */
    ComponentType mNextComponentType = 0;

public:

    /**
     * @brief Supprime les composants d'une entité dans tous les tableaux.
     * @param entity Entité à nettoyer
     */
    void EntityDestroyed(Entity entity);

    /**
     * @brief Enregistre un nouveau type de composant.
     *
     * Doit être appelé une seule fois par type T.
     */
    template<typename T>
    void RegisterComponent()
    {
        std::type_index typeName(typeid(T));

        assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Component type already registered.");

        mComponentTypes[typeName] = mNextComponentType;
        mComponentArrays[typeName] = std::make_unique<ComponentArray<T>>();

        ++mNextComponentType;
        assert(mNextComponentType <= MAX_COMPONENTS && "Exceeded maximum number of components.");
    }

    /**
     * @brief Ajoute un composant à une entité.
     * @param entity Entité cible
     * @param component Composant à ajouter
     */
    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        std::type_index typeName(typeid(T));

        auto componentArray = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
        assert(componentArray && "Component not registered.");

        componentArray->InsertData(entity, component);
    }

    /**
     * @brief Supprime un composant d'une entité.
     * @param entity Entité cible
     */
    template<typename T>
    void RemoveComponent(Entity entity) {
        std::type_index typeName(typeid(T));

        auto componentArray = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
        assert(componentArray && "Component not registered.");

        componentArray->RemoveData(entity);
    }

    /**
     * @brief Récupère une référence au composant d'une entité.
     * @param entity Entité cible
     * @return Référence au composant
     */
    template<typename T>
    T& GetComponent(Entity entity)
    {
        std::type_index typeName(typeid(T));

        auto componentArray = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
        assert(componentArray && "Component not registered.");

        return componentArray->GetData(entity);
    }

    /**
     * @brief Retourne l'ID associé au type de composant T.
     * @return ID du composant
     */
    template<typename T>
    ComponentType GetComponentType()
    {
        std::type_index typeName(typeid(T));
        assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered.");

        return mComponentTypes[typeName];
    }
};
