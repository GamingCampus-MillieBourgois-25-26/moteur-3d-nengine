#include "Engine/ECS/ECS_Coordinator.h"

void Coordinator::Init()
{
    mComponentManager = std::make_unique<ComponentManager>();
    mEntityManager = std::make_unique<EntityManager>();
    mSystemManager = std::make_unique<SystemManager>();
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

Entity Coordinator::CreateEntity()
{
    return mEntityManager->CreateEntity();
}

void Coordinator::DestroyEntity(Entity entity)
{
    mEntityManager->DestroyEntity(entity);
    mComponentManager->EntityDestroyed(entity);
    mSystemManager->EntityDestroyed(entity);
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

template<typename T>
void Coordinator::RegisterComponent()
{
    mComponentManager->RegisterComponent<T>();
}

template<typename T>
void Coordinator::AddComponent(Entity entity, T component)
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
void Coordinator::RemoveComponent(Entity entity)
{
    assert(mEntityToIndex.find(entity) != mEntityToIndex.end()); // “Je ne retire le composant que si l’entité possède ce composant.”
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
T& Coordinator::GetComponent(Entity entity)
{
    return mComponentManager->GetComponent<T>(entity);
}

template<typename T>
ComponentType Coordinator::GetComponentType()
{
    return mComponentManager->GetComponentType<T>();
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

template<typename T>
std::shared_ptr<T> Coordinator::RegisterSystem()
{
    return mSystemManager->RegisterSystem<T>();
}

template<typename T>
void Coordinator::SetSystemSignature(Signature signature)
{
    mSystemManager->SetSignature<T>(signature);
}