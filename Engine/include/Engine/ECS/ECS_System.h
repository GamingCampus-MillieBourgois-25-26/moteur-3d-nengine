#pragma once
#include "Engine/ECS/ECS_Types.h"
#include <set> 

/*
    System
    ------
    Classe de base pour tous les systèmes (MovementSystem, RenderSystem, etc.)

    Chaque système contient :
    - une liste d'entités compatibles (mEntities)
      -> alimentée automatiquement par le SystemManager

    Les systèmes ne stockent pas de données.
    Ils ne contiennent que de la logique (Update, Render, etc.)
*/

// System : contient la logique (traite les entites qui ont des composants pour pouvoir bouger et influer sur le monde
// logique + liste d'entites compatibles

class System // chaque systeme contient une liste d'entites correspondant a sa signature
{
public:
    // Ensemble des entités que ce système doit traiter
    std::set<Entity> mEntities;
};