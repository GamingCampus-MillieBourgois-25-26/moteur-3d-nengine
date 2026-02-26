#pragma once
#include "Engine/ECS/ECS_Types.h"
#include <set> 

// System : contient la logique (traite les entites qui ont des composants pour pouvoir bouger et influer sur le monde
// logique + liste d'entites compatibles

class System // chaque systeme contient une liste d'entites correspondant a sa signature
{
public:
    std::set<Entity> mEntities;
};