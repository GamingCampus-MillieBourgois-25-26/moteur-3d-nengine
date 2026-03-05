#pragma once
#include "Engine/ECS/ECS_Types.h"
#include <set> 

/**
 * @brief Classe de base pour tous les systèmes ECS.
 *
 * Un système contient uniquement de la logique (Update, Render, etc.)
 * et une liste d'entités compatibles, alimentée automatiquement
 * par le SystemManager en fonction des signatures.
 *
 * Les systèmes ne stockent aucune donnée métier.
 */
class System
{
public:
    /** @brief Ensemble des entités que ce système doit traiter. */
    std::set<Entity> mEntities;
};
