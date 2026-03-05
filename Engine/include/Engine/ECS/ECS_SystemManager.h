#pragma once
#include <iostream>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <cassert>
#include "Engine/ECS/ECS_Types.h"
#include "Engine/ECS/ECS_System.h"

/**
 * @brief Gestionnaire des systèmes ECS.
 *
 * Rôle :
 * - Enregistrer les systèmes (MovementSystem, RenderSystem, etc.)
 * - Associer une signature à chaque système
 * - Ajouter ou retirer automatiquement les entités des systèmes
 *   en fonction de leur signature
 *
 * C’est le lien entre entités, composants et logique.
 */
class SystemManager
{
private:
    /** @brief Signature requise pour chaque système. */
    std::unordered_map<std::type_index, Signature> mSignatures;

    /** @brief Liste des systèmes enregistrés. */
    std::unordered_map<std::type_index, std::shared_ptr<System>> mSystems;

public:
    SystemManager() = default;
    ~SystemManager() = default;

    /**
     * @brief Enregistre un système T.
     * @tparam T Type du système
     * @return Pointeur partagé vers l’instance du système
     */
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        std::type_index typeName = typeid(T);

        assert(mSystems.find(typeName) == mSystems.end() && "System already registered.");

        auto system = std::make_shared<T>();
        mSystems[typeName] = system;

        return system;
    }

    /**
     * @brief Associe une signature à un système T.
     * @tparam T Type du système
     * @param signature Signature requise
     */
    template<typename T>
    void SetSignature(Signature signature)
    {
        std::type_index typeName = typeid(T);
        assert(mSystems.find(typeName) != mSystems.end() && "System not registered.");

        mSignatures[typeName] = signature;
    }

    /**
     * @brief Appelé lorsqu’une entité est détruite.
     *
     * Retire l’entité de tous les systèmes.
     *
     * @param entity Entité détruite
     */
    void EntityDestroyed(Entity entity);

    /**
     * @brief Appelé lorsqu’une entité gagne ou perd un composant.
     *
     * Met à jour automatiquement sa présence dans les systèmes
     * en fonction de sa signature.
     *
     * @param entity Entité concernée
     * @param entitySignature Nouvelle signature de l’entité
     */
    void EntitySignatureChanged(Entity entity, Signature entitySignature);
};
