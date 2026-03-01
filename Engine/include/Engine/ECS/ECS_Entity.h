#pragma once
#include <queue>
#include <array>
#include <cstdint>
#include <cassert>
#include "Engine/ECS/ECS_Types.h"

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

/*
    ENTITY MANAGER — Rôle :
    -----------------------
    - Distribuer des IDs d'entités (0 -> MAX_ENTITIES)
    - Recycler les IDs détruits
    - Stocker la signature de chaque entité
    - Garantir que les entités sont valides

    Une entité = un simple entier (ID)
    Une signature = un bitset indiquant quels composants elle possède
*/

class EntityManager // role : distribuer des IDs, Recycler les IDs detruits, Stocker la signature de chaque entite
{
private:

    // IDs disponibles (libres). On pioche dedans pour créer des entités.
    std::queue<Entity> mAvailableEntities;

    // Signature de chaque entité (Transform, MeshRenderer, etc.)
    std::array<Signature, MAX_ENTITIES> mSignatures{};

    // Nombre d'entités actuellement vivantes
    std::uint32_t mLivingEntityCount = 0;

public:

    EntityManager();

    // Crée une entité en prenant un ID libre dans la queue
    Entity CreateEntity(); // prend un ID dans la queue
    
    // Détruit une entité : reset sa signature et remet son ID dans la queue
    void DestroyEntity(Entity entity); // remet l'ID dans la queue

    // Associe une signature à une entité
    void SetSignature(Entity entity, Signature signature);

    // Récupère la signature d'une entité
    Signature GetSignature(Entity entity) const;
};