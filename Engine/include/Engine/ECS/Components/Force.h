#pragma once
#include "MathsLib/Vector3.h"

/*
    Force
    -----
    Composant decrivant les forces et impulsions a appliquer
    sur le rigid body d'une entite.

    Modes :
    - Force     : force continue (Newton), appliquee chaque frame
    - Impulse   : impulsion instantanee (Newton*seconde), appliquee une seule fois

    Le ForceSystem lit ce composant et appelle les fonctions
    Bullet correspondantes sur le btRigidBody.
*/

enum class ForceMode
{
    Force,      // Force continue (appliquee chaque frame)
    Impulse     // Impulsion instantanee (appliquee une seule fois puis remise a zero)
};

struct Force
{
    Force() = default;

    // Vecteur de force / impulsion lineaire
    MathsLib::Vector3<float> force = { 0.0f, 0.0f, 0.0f };

    // Vecteur de torque / impulsion angulaire
    MathsLib::Vector3<float> torque = { 0.0f, 0.0f, 0.0f };

    // Mode d'application
    ForceMode mode = ForceMode::Force;

    // Indique si le composant a des forces en attente
    bool active = false;
};