#pragma once
#include <array> 
#include <unordered_map> 
#include "Engine/ECS/ECS_Types.h"
#include "Engine/ECS/ECS_IComponentArray.h"

template<typename T>
class ComponentArray : public IComponentArray
{
public:
    void InsertData(Entity entity, T component);
    void RemoveData(Entity entity);
    T& GetData(Entity entity);

    void EntityDestroyed(Entity entity) override;

private:
    std::array<T, MAX_ENTITIES> mComponentArray; // stockage dense
    std::unordered_map<Entity, size_t> mEntityToIndex; // ou est le composant d'une entite
    std::unordered_map<size_t, Entity> mIndexToEntity; // a qui appartient une case
    size_t mSize = 0; // nbr reel de composants actifs
};