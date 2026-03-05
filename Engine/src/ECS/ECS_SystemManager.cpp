#include "Engine/ECS/ECS_SystemManager.h"

/*
    Quand une entité est détruite :
    - elle doit être retirée de tous les systèmes
*/

void SystemManager::EntityDestroyed(Entity entity)
{
    for (auto const& pair : mSystems)
    {
        auto const& system = pair.second;
        system->mEntities.erase(entity);
    }
}

/*
    Quand une entité change de signature (ajout/suppression de composant) :
    - on vérifie pour chaque système si l'entité correspond à sa signature
    - si oui -> on ajoute l'entité au système
    - sinon -> on la retire
*/

void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
{
    for (auto const& pair : mSystems)
    {
        auto const& type = pair.first;
        auto const& system = pair.second;
        auto const& systemSignature = mSignatures[type];

        // Vérifie si l'entité possède tous les composants requis
        if ((entitySignature & systemSignature) == systemSignature)
            system->mEntities.insert(entity);
        else
            system->mEntities.erase(entity);
    }
}