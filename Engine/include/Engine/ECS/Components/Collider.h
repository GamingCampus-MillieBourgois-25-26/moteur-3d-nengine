#pragma once
#include "MathsLib/Vector3.h"

/*
    Collider
    --------
    Composant decrivant la forme de collision d'une entite.

    ShapeType :
    - Box     : demi-dimensions (halfExtents)
    - Sphere  : rayon (radius)
    - Capsule : rayon + hauteur (radius, height)

    Le ColliderSystem lit ce composant pour creer
    la btCollisionShape correspondante dans Bullet.
*/

enum class ColliderShapeType
{
    Box,
    Sphere,
    Capsule
};

struct Collider
{
    Collider() = default;

    ColliderShapeType shapeType = ColliderShapeType::Box;

    // Box : demi-dimensions sur chaque axe
    MathsLib::Vector3<float> halfExtents = { 0.5f, 0.5f, 0.5f };

    // Sphere / Capsule : rayon
    float radius = 0.5f;

    // Capsule : hauteur du cylindre interne
    float height = 1.0f;

    // Masse du rigid body (0 = statique)
    float mass = 1.0f;

    // Indique si le shape a deja ete cree par le ColliderSystem
    bool initialized = false;
};