#pragma once
#include "MathsLib/Vector3.h"
#include "MathsLib/Quaternion.h"

/**
 * @brief Composant de vitesse.
 *
 * Représente la vitesse linéaire d'une entité
 * dans l'espace 3D.
 */
struct Velocity
{
    /** @brief Vecteur de vitesse de l'entité. */
    MathsLib::Vector3<float> velocity;
};
