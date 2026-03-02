#pragma once
#include <iostream>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <cassert>
#include "Engine/ECS/ECS_Types.h"
#include "Engine/ECS/ECS_System.h"

/*
    SystemManager
    -------------
    Rôle :
    - Enregistrer les systèmes (MovementSystem, RenderSystem...)
    - Associer une signature à chaque système
    - Ajouter / retirer automatiquement les entités des systèmes
      en fonction de leur signature

    C’est le "cerveau" qui connecte entités <-> systèmes.
*/

class SystemManager 
{
private:

    // Signature requise pour chaque système
    std::unordered_map<std::type_index, Signature> mSignatures;

    // Liste des systèmes enregistrés
    std::unordered_map<std::type_index, std::shared_ptr<System>> mSystems;

public:
    SystemManager() = default; 
    ~SystemManager() = default;

    // Enregistre un système T et retourne un shared_ptr vers celui-ci
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        std::type_index typeName = typeid(T);

        // Un système ne peut être enregistré qu'une seule fois
        assert(mSystems.find(typeName) == mSystems.end() && "System already registered.");

        // Création du système
        auto system = std::make_shared<T>();
        mSystems[typeName] = system;

        return system;
    }

    // Associe une signature à un système T
    template<typename T>
    void SetSignature(Signature signature)
    {
        std::type_index typeName = typeid(T);
        assert(mSystems.find(typeName) != mSystems.end() && "System not registered.");

        mSignatures[typeName] = signature;
    }

    // Appelé quand une entité est détruite
    void EntityDestroyed(Entity entity);

    // Appelé quand une entité gagne ou perd un composant
    void EntitySignatureChanged(Entity entity, Signature entitySignature);
};