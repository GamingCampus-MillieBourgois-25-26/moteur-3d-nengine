#pragma once
#include <array>
#include <unordered_map>
#include <cassert>
#include "Engine/ECS/ECS_Types.h"
#include "Engine/ECS/ECS_IComponentArray.h"

/**
 * @brief Tableau contigu stockant tous les composants d’un type T.
 *
 * Structure optimisée pour l’ECS :
 * - accès O(1)
 * - suppression O(1) via swap avec le dernier élément
 * - données contiguës (cache-friendly)
 *
 * @tparam T Type du composant stocké
 */
template<typename T>
class ComponentArray : public IComponentArray
{
public:
    /** @brief Tableau contigu contenant les composants. */
    std::array<T, MAX_ENTITIES> mComponentArray;

    /** @brief Map entité -> index dans le tableau. */
    std::unordered_map<Entity, size_t> mEntityToIndex;

    /** @brief Map index -> entité (utile pour le swap & delete). */
    std::unordered_map<size_t, Entity> mIndexToEntity;

    /** @brief Nombre de composants actuellement stockés. */
    size_t mSize = 0;

    /**
     * @brief Ajoute un composant pour une entité.
     * @param entity Entité cible
     * @param component Composant à insérer
     */
    void InsertData(Entity entity, T component)
    {
        assert(mSize < MAX_ENTITIES && "Too many components of this type.");
        assert(mEntityToIndex.find(entity) == mEntityToIndex.end());

        size_t newIndex = mSize;

        mEntityToIndex[entity] = newIndex;
        mIndexToEntity[newIndex] = entity;
        mComponentArray[newIndex] = component;

        ++mSize;
    }

    /**
     * @brief Supprime un composant en O(1) via swap avec le dernier élément.
     * @param entity Entité dont le composant doit être supprimé
     */
    void RemoveData(Entity entity)
    {
        assert(mEntityToIndex.find(entity) != mEntityToIndex.end());

        size_t indexOfRemovedEntity = mEntityToIndex[entity];
        size_t indexOfLastElement = mSize - 1;

        mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

        Entity entityOfLastElement = mIndexToEntity[indexOfLastElement];

        mEntityToIndex[entityOfLastElement] = indexOfRemovedEntity;
        mIndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

        mEntityToIndex.erase(entity);
        mIndexToEntity.erase(indexOfLastElement);

        --mSize;
    }

    /**
     * @brief Accède au composant d’une entité.
     * @param entity Entité cible
     * @return Référence au composant
     */
    T& GetData(Entity entity)
    {
        assert(mEntityToIndex.find(entity) != mEntityToIndex.end());
        return mComponentArray[mEntityToIndex[entity]];
    }

    /**
     * @brief Supprime automatiquement le composant si l’entité est détruite.
     * @param entity Entité détruite
     */
    void EntityDestroyed(Entity entity) override
    {
        if (mEntityToIndex.find(entity) != mEntityToIndex.end())
        {
            RemoveData(entity);
        }
    }
};
