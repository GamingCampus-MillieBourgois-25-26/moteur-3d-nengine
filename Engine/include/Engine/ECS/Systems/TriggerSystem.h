#pragma once
#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Trigger.h"
#include "Engine/ECS/Systems/PhysicsBodySystem.h"
#include "Engine/ECS/Components/Collider.h"
#include "Engine/ECS/Components/Transform.h"

#include <btBulletDynamicsCommon.h>
#include <unordered_set>
#include <unordered_map>
#include <memory>

/*
    TriggerSystem
    -------------
    Systeme responsable de detecter les entrees/sorties de zones trigger.

    Responsabilites :
    - Configurer les rigid bodies des entites Trigger en mode ghost
      (CF_NO_CONTACT_RESPONSE) pour desactiver la reponse physique
    - A chaque frame, parcourir les manifolds du btDispatcher
      pour trouver les paires impliquant un trigger
    - Comparer avec l'etat precedent pour determiner
      OnEnter / OnStay / OnExit
    - Appeler les callbacks correspondants sur le composant Trigger

    Ce systeme traite les entites ayant :
    - Transform + Collider + Trigger
*/

class TriggerSystem : public System
{
public:
    // Associe le PhysicsBodySystem pour acceder au monde Bullet
    void Init(std::shared_ptr<PhysicsBodySystem> physicsSystem);

    // Detecte les overlaps et appelle les callbacks
    void Update(Coordinator& coord);

private:
    // Configure un rigid body en mode ghost (pas de reponse physique)
    void ConfigureAsTrigger(Entity entity, Coordinator& coord);

    // Retrouve l'Entity associee a un btCollisionObject
    Entity FindEntityFromBody(const btCollisionObject* body) const;

    // Reference vers le PhysicsBodySystem (proprietaire du monde Bullet)
    std::shared_ptr<PhysicsBodySystem> m_physicsSystem;
};
