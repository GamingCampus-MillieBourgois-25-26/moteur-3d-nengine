#include "Engine/ECS/Systems/ForceSystem.h"
#include <iostream>

void ForceSystem::Init(std::shared_ptr<PhysicsBodySystem> physicsSystem)
{
    m_physicsSystem = physicsSystem;
    std::cout << "ForceSystem initialized.\n";
}

void ForceSystem::Update(Coordinator& coord)
{
    if (!m_physicsSystem)
        return;

    for (auto entity : mEntities)
    {
        auto& force = coord.GetComponent<Force>(entity);

        // Ne rien faire si le composant n'est pas actif
        if (!force.active)
            continue;

        // Verifier que l'entite possede un rigid body
        btRigidBody* rb = m_physicsSystem->GetRigidBody(entity);
        if (!rb)
            continue;

        // Reveiller le body pour que Bullet prenne en compte la force
        rb->activate(true);

        btVector3 btForce(force.force.x, force.force.y, force.force.z);
        btVector3 btTorque(force.torque.x, force.torque.y, force.torque.z);

        switch (force.mode)
        {
        case ForceMode::Force:
            // Force continue : appliquee a chaque frame par Bullet
            if (btForce.length2() > 0.0f)
                rb->applyCentralForce(btForce);
            if (btTorque.length2() > 0.0f)
                rb->applyTorque(btTorque);
            break;

        case ForceMode::Impulse:
            // Impulsion instantanee : appliquee une seule fois
            if (btForce.length2() > 0.0f)
                rb->applyCentralImpulse(btForce);
            if (btTorque.length2() > 0.0f)
                rb->applyTorqueImpulse(btTorque);

            // Remettre a zero apres application
            force.force  = { 0.0f, 0.0f, 0.0f };
            force.torque = { 0.0f, 0.0f, 0.0f };
            force.active = false;
            break;
        }
    }
}