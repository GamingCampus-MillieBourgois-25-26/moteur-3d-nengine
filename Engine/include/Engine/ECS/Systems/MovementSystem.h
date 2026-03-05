#pragma once
#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Velocity.h"
#include "MathsLib/Quaternion.h"

/**
 * @brief Système de mouvement des entités.
 *
 * Met à jour la position et la rotation des entités
 * possédant les composants Transform et Velocity.
 */
class MovementSystem : public System
{
public:
    /** @brief Vitesse de rotation sur l'axe X. */
    float rotationSpeedX = 0.0f;

    /** @brief Vitesse de rotation sur l'axe Y. */
    float rotationSpeedY = 10.0f;

    /** @brief Vitesse de rotation sur l'axe Z. */
    float rotationSpeedZ = 1.0f;

    /**
     * @brief Met à jour toutes les entités compatibles.
     * @param coord Coordinateur ECS
     * @param dt Temps écoulé depuis la dernière mise à jour
     */
    void Update(Coordinator& coord, float dt);
};
