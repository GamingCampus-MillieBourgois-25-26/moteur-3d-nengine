#pragma once
#include <array>
#include <unordered_map>
#include <cassert>
#include "Engine/ECS/ECS_Types.h"
#include "Engine/ECS/ECS_IComponentArray.h"

template<typename T>
class ComponentArray : public IComponentArray
{
public:
    std::array<T, MAX_ENTITIES> mComponentArray;
    std::unordered_map<Entity, size_t> mEntityToIndex;
    std::unordered_map<size_t, Entity> mIndexToEntity;
    size_t mSize = 0;

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

    void RemoveData(Entity entity)
    {
        assert(mEntityToIndex.find(entity) != mEntityToIndex.end());

        size_t indexOfRemovedEntity = mEntityToIndex[entity];
        size_t indexOfLastElement = mSize - 1;

        // Move last element
        mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

        Entity entityOfLastElement = mIndexToEntity[indexOfLastElement];

        mEntityToIndex[entityOfLastElement] = indexOfRemovedEntity;
        mIndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

        mEntityToIndex.erase(entity);
        mIndexToEntity.erase(indexOfLastElement);

        --mSize;
    }

    T& GetData(Entity entity)
    {
        assert(mEntityToIndex.find(entity) != mEntityToIndex.end());
        return mComponentArray[mEntityToIndex[entity]];
    }

    void EntityDestroyed(Entity entity) override
    {
        if (mEntityToIndex.find(entity) != mEntityToIndex.end())
        {
            RemoveData(entity);
        }
    }
};
