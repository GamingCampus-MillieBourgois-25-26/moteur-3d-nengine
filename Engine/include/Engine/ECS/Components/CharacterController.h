#pragma once
#include "MathsLib/Vector3.h"
#include "Engine/ECS/ECS_Types.h"

/*
    CharacterController
    --------------------
    Composant decrivant un personnage jouable controle par le moteur physique.

    Utilise un btKinematicCharacterController (ghost object + capsule shape)
    pour gerer le deplacement, le saut et la detection du sol.

    Le CharacterControllerSystem lit ce composant et cree le
    btKinematicCharacterController correspondant dans le monde Bullet
    via PhysicsBodySystem.
*/

struct CharacterController
{
    CharacterController() = default;

    // Dimensions de la capsule du personnage
    float capsuleRadius = 0.4f;
    float capsuleHeight = 1.0f;

    // Vitesse de deplacement
    float walkSpeed = 5.0f;

    // Force du saut
    float jumpSpeed = 6.0f;

    // Pente maximale franchissable (en radians)
    float maxSlopeAngle = 0.78f; // ~45 degres

    // Hauteur maximale d'une marche franchissable
    float stepHeight = 0.35f;

    // Gravite appliquee au personnage (negative = vers le bas)
    float gravity = -9.81f;

    // Direction de deplacement souhaitee (definie chaque frame par le gameplay)
    MathsLib::Vector3<float> walkDirection = { 0.0f, 0.0f, 0.0f };

    // Demande de saut (consommee par le systeme)
    bool jump = false;

    // Indique si le personnage touche le sol (mis a jour par le systeme)
    bool onGround = false;

    // Indique si le controller a deja ete cree par le CharacterControllerSystem
    bool initialized = false;
};