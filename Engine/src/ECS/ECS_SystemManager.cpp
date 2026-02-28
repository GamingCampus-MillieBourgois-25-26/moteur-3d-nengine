#include "Engine/ECS/ECS_SystemManager.h"
/*
template<typename T>
std::shared_ptr<T> SystemManager::RegisterSystem()
{
    std::type_index typeName = typeid(T);
    assert(mSystems.find(typeName) == mSystems.end() && "System already registered.");

    auto system = std::make_shared<T>();
    mSystems[typeName] = system;
    return system;
}

template<typename T>
void SystemManager::SetSignature(Signature signature)
{
    std::type_index typeName = typeid(T);
    assert(mSystems.find(typeName) != mSystems.end() && "System not registered.");

    mSignatures[typeName] = signature;
}
*/
void SystemManager::EntityDestroyed(Entity entity)
{
    for (auto const& pair : mSystems)
    {
        auto const& system = pair.second;
        system->mEntities.erase(entity);
    }
}

// Si l'entite possede tous les composants requis par le systeme
void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
{
    for (auto const& pair : mSystems)
    {
        auto const& type = pair.first;
        auto const& system = pair.second;
        auto const& systemSignature = mSignatures[type];

        if ((entitySignature & systemSignature) == systemSignature)
            system->mEntities.insert(entity);
        else
            system->mEntities.erase(entity);
    }
}