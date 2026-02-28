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

// test déclaration anticipée pour voir si ca regle les bugs chelou

//class ComponentManager;
//class EntityManager;
//class SystemManager;

// Coordinator : il permet de tout regler 

class Coordinator
{
public:

    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<SystemManager> mSystemManager;

    void Init();

    Entity CreateEntity();
    void DestroyEntity(Entity entity);

    /*template<typename T>
    void RegisterComponent();

    template<typename T>
    void AddComponent(Entity entity, T component);

    template<typename T>
    void RemoveComponent(Entity entity);

    template<typename T>
    T& GetComponent(Entity entity);

    template<typename T>
    ComponentType GetComponentType();

    template<typename T>
    std::shared_ptr<T> RegisterSystem();

    template<typename T>
    void SetSystemSignature(Signature signature);*/

    template<typename T>
    void RegisterComponent()
    {
        mComponentManager->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        assert(entity < MAX_ENTITIES && "Invalid entity.");
        mComponentManager->AddComponent<T>(entity, component);

        // Mettre à jour la signature de l'entité
        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), true);
        mEntityManager->SetSignature(entity, signature);

        // Informer les systèmes que la signature a changé
        mSystemManager->EntitySignatureChanged(entity, signature);
    }

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

    template<typename T>
    T& GetComponent(Entity entity)
    {
        return mComponentManager->GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        return mComponentManager->GetComponentType<T>();
    }

    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////

    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return mSystemManager->RegisterSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        mSystemManager->SetSignature<T>(signature);
    }
};