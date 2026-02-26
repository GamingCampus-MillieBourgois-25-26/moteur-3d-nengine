#pragma once
#include <iostream>
#include "MathsLib/Quaternion.h"
#include "MathsLib/Vector3.h"
#include <queue>
#include <map>
#include <thread>
#include <bitset>

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