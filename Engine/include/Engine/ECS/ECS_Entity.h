#pragma once
#include <iostream>
#include "MathsLib/Quaternion.h"
#include "MathsLib/Vector3.h"
#include <queue>
#include <map>
#include <thread>
#include <bitset>

// Signature : bitset qui dit quels composants une entite possède (0111)

using Signature = std::bitset<MAX_COMPONENTS>;

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

using Entity = std::uint32_t; // pas la peine de faire une classe car on a juste besoin de stocker l'ID. // Entity player = 1; Entity enemy = 2;
constexpr std::uint32_t MAX_ENTITIES = 20;

class EntityManager // role : distribuer des IDs, Recycler les IDs detruits, Stocker la signature de chaque entite
{
public:
    Entity CreateEntity(); // prend un ID dans la queue
    void DestroyEntity(Entity entity); // remet l'ID dans la queue

    void SetSignature(Entity entity, Signature signature);
    Signature GetSignature(Entity entity);

private:
    std::queue<Entity> mAvailableEntities; // contient les IDs libres 
    std::array<Signature, MAX_ENTITIES> mSignatures{}; // contient la signature
    std::uint32_t mLivingEntityCount = 0;
};

