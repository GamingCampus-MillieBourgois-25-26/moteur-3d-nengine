#pragma once
#include "MathsLib/Vector3.h"
#include "MathsLib/Quaternion.h"

/**
 * @brief Composant de transformation spatiale.
 *
 * Représente la position, la rotation et l'échelle
 * d'une entité dans l'espace 3D.
 */
struct Transform
{
    /** @brief Construit un transform par défaut. */
    Transform() = default;

    /** @brief Position de l'entité. */
    MathsLib::Vector3<float> position;

    /** @brief Rotation de l'entité. */
    MathsLib::Quaternion<float> rotation;

    /** @brief Échelle de l'entité. */
    MathsLib::Vector3<float> scale;
};
