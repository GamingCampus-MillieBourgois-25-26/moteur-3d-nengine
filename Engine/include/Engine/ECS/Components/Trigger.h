#pragma once
#include "Engine/ECS/ECS_Types.h"
#include <functional>
#include <unordered_set>

/*
    Trigger
    -------
    Composant qui marque un collider comme zone de detection (trigger).

    Un trigger ne genere pas de reponse physique (pas de rebond),
    mais detecte les entrees et sorties d'autres entites.

    Le TriggerSystem lit ce composant pour :
    - Configurer le btRigidBody comme ghost (CF_NO_CONTACT_RESPONSE)
    - Detecter les paires de collision via le btDispatcher
    - Appeler les callbacks OnEnter / OnStay / OnExit
*/

struct Trigger
{
    Trigger() = default;

    // Callbacks utilisateur (optionnels)
    std::function<void(Entity /*self*/, Entity /*other*/)> onEnter = nullptr;
    std::function<void(Entity /*self*/, Entity /*other*/)> onStay  = nullptr;
    std::function<void(Entity /*self*/, Entity /*other*/)> onExit  = nullptr;

    // Etat interne : entites actuellement dans la zone (gere par TriggerSystem)
    std::unordered_set<Entity> currentOverlaps;

    // Indique si le rigid body a deja ete configure en ghost
    bool initialized = false;
};