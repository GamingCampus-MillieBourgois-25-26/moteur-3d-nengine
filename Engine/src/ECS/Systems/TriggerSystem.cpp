#include "Engine/ECS/Systems/TriggerSystem.h"
#include "Engine/ECS/Components/Collider.h"
#include "Engine/ECS/Components/Transform.h"
#include <iostream>

void TriggerSystem::Init(std::shared_ptr<PhysicsBodySystem> physicsSystem)
{
    m_physicsSystem = physicsSystem;
    std::cout << "TriggerSystem initialized.\n";
}

void TriggerSystem::Update(Coordinator& coord)
{
    if (!m_physicsSystem)
        return;

    // 1. Configurer les nouveaux triggers en mode ghost
    for (auto entity : mEntities)
    {
        auto& trigger = coord.GetComponent<Trigger>(entity);

        if (!trigger.initialized)
        {
            ConfigureAsTrigger(entity, coord);
            trigger.initialized = true;
        }
    }

    // 2. Collecter les overlaps actuels pour chaque entite trigger
    //    Clef = entite trigger, Valeur = ensemble des entites en contact
    std::unordered_map<Entity, std::unordered_set<Entity>> currentFrameOverlaps;

    // Acceder au dispatcher via le monde Bullet
    btDynamicsWorld* world = m_physicsSystem->GetDynamicsWorld();
    if (!world)
        return;

    btDispatcher* dispatcher = world->getDispatcher();
    int numManifolds = dispatcher->getNumManifolds();

    for (int i = 0; i < numManifolds; ++i)
    {
        btPersistentManifold* manifold = dispatcher->getManifoldByIndexInternal(i);
        if (manifold->getNumContacts() == 0)
            continue;

        const btCollisionObject* bodyA = manifold->getBody0();
        const btCollisionObject* bodyB = manifold->getBody1();

        Entity entityA = FindEntityFromBody(bodyA);
        Entity entityB = FindEntityFromBody(bodyB);

        if (entityA == static_cast<Entity>(-1) || entityB == static_cast<Entity>(-1))
            continue;

        // Verifier si A ou B est un trigger
        bool aIsTrigger = mEntities.count(entityA) > 0;
        bool bIsTrigger = mEntities.count(entityB) > 0;

        if (aIsTrigger)
            currentFrameOverlaps[entityA].insert(entityB);

        if (bIsTrigger)
            currentFrameOverlaps[entityB].insert(entityA);
    }

    // 3. Comparer avec l'etat precedent pour determiner Enter / Stay / Exit
    for (auto entity : mEntities)
    {
        auto& trigger = coord.GetComponent<Trigger>(entity);

        auto& newOverlaps = currentFrameOverlaps[entity]; // peut etre vide
        auto& oldOverlaps = trigger.currentOverlaps;

        // OnEnter : present maintenant, absent avant
        for (Entity other : newOverlaps)
        {
            if (oldOverlaps.find(other) == oldOverlaps.end())
            {
                std::cout << "TriggerSystem: entity " << entity
                          << " OnEnter with entity " << other << "\n";
                if (trigger.onEnter)
                    trigger.onEnter(entity, other);
            }
            else
            {
                // OnStay : present maintenant et avant
                if (trigger.onStay)
                    trigger.onStay(entity, other);
            }
        }

        // OnExit : absent maintenant, present avant
        for (Entity other : oldOverlaps)
        {
            if (newOverlaps.find(other) == newOverlaps.end())
            {
                std::cout << "TriggerSystem: entity " << entity
                          << " OnExit with entity " << other << "\n";
                if (trigger.onExit)
                    trigger.onExit(entity, other);
            }
        }

        // Mettre a jour l'etat
        oldOverlaps = newOverlaps;
    }
}

void TriggerSystem::ConfigureAsTrigger(Entity entity, Coordinator& coord)
{
    if (!m_physicsSystem->HasRigidBody(entity))
        return;

    btRigidBody* body = m_physicsSystem->GetRigidBody(entity);
    if (!body)
        return;

    // Desactiver la reponse physique : le body detecte les collisions
    // mais ne genere pas de forces de contact
    body->setCollisionFlags(
        body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE
    );

    // Stocker l'entity dans le user pointer pour le retrouver plus tard
    body->setUserIndex(static_cast<int>(entity));

    std::cout << "TriggerSystem: configured entity " << entity << " as trigger.\n";
}

Entity TriggerSystem::FindEntityFromBody(const btCollisionObject* body) const
{
    if (!body)
        return static_cast<Entity>(-1);

    int idx = body->getUserIndex();
    if (idx >= 0)
        return static_cast<Entity>(idx);

    return static_cast<Entity>(-1);
}