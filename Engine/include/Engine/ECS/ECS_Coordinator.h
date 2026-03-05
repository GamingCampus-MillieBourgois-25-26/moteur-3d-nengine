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

/**
 * @brief Façade principale de l’ECS.
 *
 * Le Coordinator regroupe :
 * - EntityManager : création et destruction d'entités
 * - ComponentManager : gestion des composants
 * - SystemManager : gestion des systèmes et signatures
 *
 * Il fournit une API simple utilisée par tout le moteur.
 */
class Coordinator
{
public:

    /** @brief Gestionnaire des composants. */
    std::unique_ptr<ComponentManager> mComponentManager;

    /** @brief Gestionnaire des entités. */
    std::unique_ptr<EntityManager> mEntityManager;

    /** @brief Gestionnaire des systèmes. */
    std::unique_ptr<SystemManager> mSystemManager;

    /**
     * @brief Initialise les trois managers internes.
     */
    void Init();

    /**
     * @brief Crée une nouvelle entité.
     * @return Identifiant de l'entité créée
     */
    Entity CreateEntity();

    /**
     * @brief Détruit une entité et nettoie ses composants.
     * @param entity Entité à détruire
     */
    void DestroyEntity(Entity entity);

    /**
     * @brief Enregistre un nouveau type de composant.
     * @tparam T Type du composant
     */
    template<typename T>
    void RegisterComponent()
    {
        mComponentManager->RegisterComponent<T>();
    }

    /**
     * @brief Ajoute un composant à une entité.
     * @tparam T Type du composant
     * @param entity Entité cible
     * @param component Composant à ajouter
     */
    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        assert(entity < MAX_ENTITIES && "Invalid entity.");

        mComponentManager->AddComponent<T>(entity, component);

        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), true);
        mEntityManager->SetSignature(entity, signature);

        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    /**
     * @brief Retire un composant d'une entité.
     * @tparam T Type du composant
     * @param entity Entité cible
     */
    template<typename T>
    void RemoveComponent(Entity entity)
    {
        mComponentManager->RemoveComponent<T>(entity);

        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), false);
        mEntityManager->SetSignature(entity, signature);

        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    /**
     * @brief Accède au composant d'une entité.
     * @tparam T Type du composant
     * @param entity Entité cible
     * @return Référence au composant
     */
    template<typename T>
    T& GetComponent(Entity entity)
    {
        return mComponentManager->GetComponent<T>(entity);
    }

    /**
     * @brief Récupère l'ID associé au type de composant T.
     * @tparam T Type du composant
     * @return ID du composant
     */
    template<typename T>
    ComponentType GetComponentType()
    {
        return mComponentManager->GetComponentType<T>();
    }

    /**
     * @brief Enregistre un système ECS.
     * @tparam T Type du système
     * @return Pointeur partagé vers le système créé
     */
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return mSystemManager->RegisterSystem<T>();
    }

    /**
     * @brief Définit la signature d'un système.
     * @tparam T Type du système
     * @param signature Signature à associer
     */
    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        mSystemManager->SetSignature<T>(signature);
    }
};
