#pragma once
#include <memory>
#include <cstdint>
#include <bitset>
#include <cassert>
#include <typeindex>
#include <unordered_map>
#include <set>

#include "Engine/ECS/ECS_Types.h"
#include "Engine/ECS/ECS_Entity.h"
#include "Engine/ECS/ECS_Component.h"
#include "Engine/ECS/ECS_ComponentArray.h"
#include "Engine/ECS/ECS_SystemManager.h"

/*
    Coordinator
    -----------
    C’est la façade principale de l’ECS.

    Il regroupe :
    - EntityManager      -> création/destruction d'entités
    - ComponentManager   -> ajout/retrait/accès aux composants
    - SystemManager      -> enregistrement des systèmes + signatures

    Le Coordinator fournit une API simple :
    - CreateEntity()
    - AddComponent<T>()
    - RegisterSystem<T>()
    - SetSystemSignature<T>()

    C’est l’interface que le moteur utilise au quotidien.
*/

class Coordinator
{
public:

    // Les trois managers internes
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<SystemManager> mSystemManager;

    // Initialise les managers
    void Init();

    // Création / destruction d'entités
    Entity CreateEntity();
    void DestroyEntity(Entity entity);

    // Enregistrement d’un type de composant
    template<typename T>
    void RegisterComponent()
    {
        mComponentManager->RegisterComponent<T>();
    }

    // Ajout d’un composant à une entité
    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        assert(entity < MAX_ENTITIES && "Invalid entity.");

        // 1. Ajouter le composant dans ComponentManager
        mComponentManager->AddComponent<T>(entity, component);

        // 2. Mettre à jour la signature de l'entité
        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), true);
        mEntityManager->SetSignature(entity, signature);

        // 3. Informer les systèmes que la signature a changé
        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    // Retrait d’un composant
    template<typename T>
    void RemoveComponent(Entity entity)
    {
        // 1. Retirer le composant
        mComponentManager->RemoveComponent<T>(entity);

        // 2. Mettre à jour la signature 
        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), false);
        mEntityManager->SetSignature(entity, signature);

        // 3. Informer les systèmes 
        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    // Accès à un composant
    template<typename T>
    T& GetComponent(Entity entity)
    {
        return mComponentManager->GetComponent<T>(entity);
    }

    // Récupère l’ID d’un type de composant
    template<typename T>
    ComponentType GetComponentType()
    {
        return mComponentManager->GetComponentType<T>();
    }

    // Enregistre un système
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return mSystemManager->RegisterSystem<T>();
    }

    // Associe une signature à un système
    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        mSystemManager->SetSignature<T>(signature);
    }
};