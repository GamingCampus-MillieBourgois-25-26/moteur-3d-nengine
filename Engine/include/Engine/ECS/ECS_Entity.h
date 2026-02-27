#pragma once
#include <queue>
#include <array>
#include <cstdint>
#include <cassert>
#include "Engine/ECS/ECS_Types.h"

// ECS - Entity Component System

/*

Entity            -> simple ID
Signature         -> bitset de composants

EntityManager     -> gère IDs + signatures
ComponentArray<T> -> stocke les composants d’un type
ComponentManager  -> gère tous les ComponentArray
System            -> classe de base
SystemManager     -> gère les systèmes
Coordinator       -> façade globale

*/

// Entity (ID)

class EntityManager // role : distribuer des IDs, Recycler les IDs detruits, Stocker la signature de chaque entite
{
public:

    EntityManager();

    Entity CreateEntity(); // prend un ID dans la queue
    void DestroyEntity(Entity entity); // remet l'ID dans la queue

    void SetSignature(Entity entity, Signature signature);
    Signature GetSignature(Entity entity) const;

private:
    std::queue<Entity> mAvailableEntities; // contient les IDs libres 
    std::array<Signature, MAX_ENTITIES> mSignatures{}; // contient la signature
    std::uint32_t mLivingEntityCount = 0;
};