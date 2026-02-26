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
#include "Engine/ECS/ECS_SystemManager.h"

// Coordinator : il permet de tout regler 

class Coordinator
{
public:
    void Init();

    Entity CreateEntity();
    void DestroyEntity(Entity entity);

    template<typename T>
    void RegisterComponent();

    template<typename T>
    void AddComponent(Entity entity, T component);

    template<typename T>
    T& GetComponent(Entity entity);

    template<typename T>
    ComponentType GetComponentType();

    template<typename T>
    std::shared_ptr<T> RegisterSystem();

    template<typename T>
    void SetSystemSignature(Signature signature);

private:
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager>    mEntityManager;
    std::unique_ptr<SystemManager>    mSystemManager;
};