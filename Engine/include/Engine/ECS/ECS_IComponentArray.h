#pragma once
#include "Engine/ECS/ECS_Types.h"

/**
 * @brief Interface de base pour tous les tableaux de composants.
 *
 * Permet au ComponentManager de manipuler des ComponentArray<T>
 * de manière polymorphique, sans connaître le type T.
 *
 * Chaque ComponentArray<T> doit implémenter la suppression
 * automatique du composant associé lorsqu'une entité est détruite.
 */
class IComponentArray
{
public:
    /** @brief Destructeur virtuel pour permettre la destruction polymorphique. */
    virtual ~IComponentArray() = default;

    /**
     * @brief Appelé lorsqu'une entité est détruite.
     *
     * Chaque implémentation doit supprimer le composant associé
     * à cette entité si elle en possède un.
     *
     * @param entity Entité détruite
     */
    virtual void EntityDestroyed(Entity entity) = 0;
};
