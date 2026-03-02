#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/ECS_Component.h"
#include "Engine/ECS/ECS_Entity.h"
#include "Engine/ECS/ECS_SystemManager.h"

/*
    Init()
    ------
    Initialise les trois managers internes.
    Obligatoire avant toute utilisation du Coordinator.
*/

void Coordinator::Init()
{
    mComponentManager = std::make_unique<ComponentManager>();
    mEntityManager = std::make_unique<EntityManager>();
    mSystemManager = std::make_unique<SystemManager>();
}

/*
    CreateEntity()
    --------------
    Demande à l’EntityManager un nouvel ID d’entité.
*/

Entity Coordinator::CreateEntity()
{
    return mEntityManager->CreateEntity();
}

/*
    DestroyEntity()
    ---------------
    Détruit une entité dans :
    - EntityManager      -> recycle l’ID
    - ComponentManager   -> supprime tous ses composants
    - SystemManager      -> la retire de tous les systèmes
*/

void Coordinator::DestroyEntity(Entity entity)
{
    mEntityManager->DestroyEntity(entity);
    mComponentManager->EntityDestroyed(entity);
    mSystemManager->EntityDestroyed(entity);
}