#pragma once
#include <iostream>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <cassert>
#include "Engine/ECS/ECS_Types.h"
#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"

class SystemManager // role : enregistrer les systemes, associer une signature a chaque systeme, mettre a jour les listes quand une signature change
{
public:
    template<typename T>
    std::shared_ptr<T> RegisterSystem();

    template<typename T>
    void SetSignature(Signature signature);

    void EntityDestroyed(Entity entity);
    void EntitySignatureChanged(Entity entity, Signature entitySignature);

private:
    std::unordered_map<std::type_index, Signature> mSignatures;
    std::unordered_map<std::type_index, std::shared_ptr<System>> mSystems;
};