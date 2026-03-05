#pragma once

#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Joint.h"
#include "Engine/ECS/Systems/PhysicsBodySystem.h"

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h>
#include <BulletDynamics/ConstraintSolver/btHingeConstraint.h>
#include <BulletDynamics/ConstraintSolver/btFixedConstraint.h>
#include <memory>
#include <unordered_map>

/*
    JointSystem
    -----------
    Systeme responsable de creer et mettre a jour les contraintes
    Bullet entre entites.

    Responsabilites :
    - Detecter les entites ayant un composant Joint non initialise
    - Creer la btTypedConstraint correspondante (Point2Point, Hinge, Fixed)
    - Ajouter la contrainte au monde Bullet via PhysicsBodySystem
    - Supprimer les contraintes quand une entite est detruite

    Le PhysicsBodySystem reste proprietaire du monde Bullet.
    Ce systeme ne fait que creer et gerer les contraintes.

    Ce systeme traite les entites ayant :
    - Joint
*/

class JointSystem : public System
{
public:
    // Associe le PhysicsBodySystem pour acceder au monde Bullet et aux btRigidBody
    void Init(std::shared_ptr<PhysicsBodySystem> physicsSystem);

    // Parcourt les entites et cree les contraintes non initialisees
    void Update(Coordinator& coord);

    // Supprime la contrainte associee a une entite
    void RemoveJoint(Entity entity);

    // Libere toutes les contraintes
    void Shutdown();

private:
    // Cree la btTypedConstraint adaptee au type du Joint
    std::unique_ptr<btTypedConstraint> CreateConstraint(
        const Joint& joint,
        btRigidBody* bodyA,
        btRigidBody* bodyB);

    // Reference vers le PhysicsBodySystem (proprietaire du monde Bullet)
    std::shared_ptr<PhysicsBodySystem> m_physicsSystem;

    // Association Entity -> contrainte Bullet
    struct JointData
    {
        std::unique_ptr<btTypedConstraint> constraint;
    };

    std::unordered_map<Entity, JointData> m_joints;
};
