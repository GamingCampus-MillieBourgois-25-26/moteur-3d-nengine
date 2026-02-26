#pragma once
#include <iostream>
#include "MathsLib/Quaternion.h"
#include "MathsLib/Vector3.h"
#include <queue>
#include <map>
#include <thread>
#include <bitset>

// System : contient la logique (traite les entites qui ont des composants pour pouvoir bouger et influer sur le monde
// logique + liste d'entites compatibles

class System // chaque systeme contient une liste d'entites correspondant a sa signature
{
public:
    std::set<Entity> mEntities;
};

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
    std::unordered_map<const char*, Signature> mSignatures;
    std::unordered_map<const char*, std::shared_ptr<System>> mSystems;
};

// Definition systeme 

class MovementSystem : public System
{
public:
    void Update(Coordinator& coord)
    {
        for (auto entity : mEntities)
        {
            auto& t = coord.GetComponent<Transform>(entity);
            auto& v = coord.GetComponent<Velocity>(entity);

            t.x += v.dx;
            t.y += v.dy;
        }
    }
};
