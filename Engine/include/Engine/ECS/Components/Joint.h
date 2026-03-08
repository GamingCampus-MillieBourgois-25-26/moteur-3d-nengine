#pragma once
#include "MathsLib/Vector3.h"
#include "Engine/ECS/ECS_Types.h"

/*
    Joint
    -----
    Composant decrivant une contrainte physique entre deux entites.

    Types de contraintes :
    - Point2Point : liaison rotule (pivot libre)
    - Hinge       : liaison pivot (rotation sur un axe)
    - Fixed       : liaison rigide (aucun degre de liberte)

    Le JointSystem lit ce composant et cree la btTypedConstraint
    correspondante dans le monde Bullet via PhysicsBodySystem.
*/

enum class JointType
{
    Point2Point,   // Rotule : rotation libre autour d'un point
    Hinge,         // Pivot  : rotation sur un axe unique
    Fixed          // Fixe   : aucun mouvement relatif
};

struct Joint
{
    Joint() = default;

    // Type de contrainte
    JointType type = JointType::Point2Point;

    // Entite cible (l'autre extremite de la contrainte)
    Entity targetEntity = 0;

    // Point d'ancrage local sur l'entite source (this entity)
    MathsLib::Vector3<float> pivotA = { 0.0f, 0.0f, 0.0f };

    // Point d'ancrage local sur l'entite cible
    MathsLib::Vector3<float> pivotB = { 0.0f, 0.0f, 0.0f };

    // Axe de rotation pour Hinge (en coordonnees locales de chaque body)
    MathsLib::Vector3<float> axisA = { 0.0f, 1.0f, 0.0f };
    MathsLib::Vector3<float> axisB = { 0.0f, 1.0f, 0.0f };

    // Limites angulaires pour Hinge (en radians)
    float lowerLimit = 0.0f;
    float upperLimit = 0.0f;

    // La contrainte casse si la force depasse ce seuil (0 = incassable)
    float breakingForce = 0.0f;

    // Indique si la contrainte a deja ete creee par le JointSystem
    bool initialized = false;
};