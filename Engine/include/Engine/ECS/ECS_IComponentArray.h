#pragma once
#include "Engine/ECS/ECS_Types.h"

/*
    IComponentArray
    ----------------
    Interface commune à tous les ComponentArray<T>.

    Pourquoi ?
    ----------
    Le ComponentManager stocke des ComponentArray<T> dans un conteneur
    polymorphique (unordered_map<type_index, unique_ptr<IComponentArray>>).

    Il doit pouvoir appeler EntityDestroyed() sur n'importe quel ComponentArray,
    sans connaître le type T.
*/

class IComponentArray // Le ComponentManager doit pouvoir appeler EntityDestroyed() sans connaître le type T.
{
public:
    virtual ~IComponentArray() = default;
    // Appelé quand une entité est détruite
    // Chaque ComponentArray<T> doit supprimer le composant associé
    virtual void EntityDestroyed(Entity entity) = 0;
};