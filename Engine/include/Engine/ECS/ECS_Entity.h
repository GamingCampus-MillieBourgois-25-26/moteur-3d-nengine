#pragma once
#include <queue>
#include <array>
#include <cstdint>
#include <cassert>
#include "Engine/ECS/ECS_Types.h"

/**
 * @brief Gestionnaire des entités ECS.
 *
 * Rôle :
 * - Distribuer des IDs d'entités (0 → MAX_ENTITIES)
 * - Recycler les IDs détruits
 * - Stocker la signature de chaque entité
 * - Garantir la validité des entités
 *
 * Une entité = un simple entier (ID)
 * Une signature = un bitset indiquant quels composants elle possède
 */
class EntityManager
{
private:
    /** @brief IDs disponibles pour la création d'entités. */
    std::queue<Entity> mAvailableEntities;

    /** @brief Signature de chaque entité (Transform, MeshRenderer, etc.). */
    std::array<Signature, MAX_ENTITIES> mSignatures{};

    /** @brief Nombre d'entités actuellement vivantes. */
    std::uint32_t mLivingEntityCount = 0;

public:
    /** @brief Construit un gestionnaire d'entités et initialise la liste d'IDs libres. */
    EntityManager();

    /**
     * @brief Crée une nouvelle entité en prenant un ID libre.
     * @return ID de l'entité créée
     */
    Entity CreateEntity();

    /**
     * @brief Détruit une entité : réinitialise sa signature et recycle son ID.
     * @param entity Entité à détruire
     */
    void DestroyEntity(Entity entity);

    /**
     * @brief Définit la signature d'une entité.
     * @param entity Entité cible
     * @param signature Signature à associer
     */
    void SetSignature(Entity entity, Signature signature);

    /**
     * @brief Récupère la signature d'une entité.
     * @param entity Entité cible
     * @return Signature de l'entité
     */
    Signature GetSignature(Entity entity) const;
};
