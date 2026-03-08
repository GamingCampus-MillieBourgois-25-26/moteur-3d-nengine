#pragma once
#include <array>
#include <unordered_map>
#include <cassert>
#include "Engine/ECS/ECS_Types.h"
#include "Engine/ECS/ECS_IComponentArray.h"

/*
    ComponentArray<T>
    -----------------
    Stocke tous les composants d’un type T.

    Structure :
    - mComponentArray : tableau contigu (rapide, cache-friendly)
    - mEntityToIndex  : map entité -> index dans le tableau
    - mIndexToEntity  : map index -> entité (pour swap & delete)
    - mSize           : nombre de composants stockés

    Objectif :
    - accès O(1)
    - suppression O(1) (swap avec le dernier)
*/

template<typename T>
class ComponentArray : public IComponentArray
{
public:
    std::array<T, MAX_ENTITIES> mComponentArray;
    std::unordered_map<Entity, size_t> mEntityToIndex;
    std::unordered_map<size_t, Entity> mIndexToEntity;
    size_t mSize = 0;

    // Ajoute un composant pour une entité
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

    // Supprime un composant (swap & delete)
    void RemoveData(Entity entity)
    {
        assert(mEntityToIndex.find(entity) != mEntityToIndex.end());

        size_t indexOfRemovedEntity = mEntityToIndex[entity];
        size_t indexOfLastElement = mSize - 1;

        // Déplace le dernier élément à la place de celui supprimé
        mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

        Entity entityOfLastElement = mIndexToEntity[indexOfLastElement];

        // Met à jour les maps
        mEntityToIndex[entityOfLastElement] = indexOfRemovedEntity;
        mIndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

        // Supprime les anciennes entrées
        mEntityToIndex.erase(entity);
        mIndexToEntity.erase(indexOfLastElement);

        --mSize;
    }

    // Accès direct au composant d'une entité
    T& GetData(Entity entity)
    {
        assert(mEntityToIndex.find(entity) != mEntityToIndex.end());
        return mComponentArray[mEntityToIndex[entity]];
    }

    // Appelé quand une entité est détruite
    void EntityDestroyed(Entity entity) override
    {
        if (mEntityToIndex.find(entity) != mEntityToIndex.end())
        {
            RemoveData(entity);
        }
    }
};
