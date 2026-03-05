#pragma once
#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Force.h"
#include "Engine/ECS/Systems/PhysicsBodySystem.h"

#include <memory>

/*
    ForceSystem
    -----------
    Systeme responsable d'appliquer des forces et impulsions
    sur les rigid bodies Bullet.

    Responsabilites :
    - Parcourir les entites ayant un composant Force actif
    - Appliquer force / impulsion lineaire via btRigidBody
    - Appliquer torque / impulsion angulaire via btRigidBody
    - Remettre a zero le composant apres application (mode Impulse)

    Le PhysicsBodySystem reste proprietaire des btRigidBody.
    Ce systeme ne fait que leur transmettre des forces.

    Ce systeme traite les entites ayant :
    - Force
*/

class ForceSystem : public System
{
public:
    // Associe le PhysicsBodySystem pour acceder aux btRigidBody
    void Init(std::shared_ptr<PhysicsBodySystem> physicsSystem);

    // Parcourt les entites et applique les forces / impulsions
    void Update(Coordinator& coord);

private:
    // Reference vers le PhysicsBodySystem (proprietaire des btRigidBody)
    std::shared_ptr<PhysicsBodySystem> m_physicsSystem;
};
