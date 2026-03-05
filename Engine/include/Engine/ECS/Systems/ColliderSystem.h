#pragma once
#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Collider.h"
#include "Engine/ECS/Systems/PhysicsBodySystem.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <LinearMath/btVector3.h>
#include <memory>

/*
    ColliderSystem
    --------------
    Systeme responsable de creer les collision shapes Bullet
    a partir du composant Collider.

    Responsabilites :
    - Detecter les entites ayant Transform + Collider non initialise
    - Creer la btCollisionShape correspondante (box, sphere, capsule)
    - Deleguer la creation du btRigidBody au PhysicsBodySystem

    Le PhysicsBodySystem reste proprietaire du monde Bullet,
    de la simulation et de la synchronisation des Transforms.
*/

class ColliderSystem : public System
{
public:
    // Associe le PhysicsBodySystem pour y enregistrer les rigid bodies
    void Init(std::shared_ptr<PhysicsBodySystem> physicsSystem);

    // Parcourt les entites avec un Collider non initialise et cree les shapes
    void Update(Coordinator& coord);

private:
    // Cree la btCollisionShape adaptee au type du Collider
    std::unique_ptr<btCollisionShape> CreateShape(const Collider& collider);

    // Reference vers le PhysicsBodySystem (proprietaire du monde Bullet)
    std::shared_ptr<PhysicsBodySystem> m_physicsSystem;
};
