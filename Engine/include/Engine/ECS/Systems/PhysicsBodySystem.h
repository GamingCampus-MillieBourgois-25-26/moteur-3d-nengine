#pragma once
#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/Transform.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <LinearMath/btVector3.h>
#include <memory>
#include <unordered_map>

/*
    PhysicsBodySystem
    -----------------
    Systeme physique utilisant Bullet Physics pour simuler les rigid bodies.

    Responsabilites :
    - Initialiser le monde physique Bullet (btDiscreteDynamicsWorld)
    - Creer / supprimer des btRigidBody pour chaque entite compatible
    - Appeler stepSimulation chaque frame
    - Synchroniser les resultats vers le composant Transform

    Ce systeme traite les entites ayant :
    - Transform
*/

class PhysicsBodySystem : public System
{
public:
    // Initialise le monde physique Bullet
    void Init();

    // Ajoute un rigid body a partir d'une shape deja creee (utilise par ColliderSystem)
    void AddRigidBody(Entity entity, Coordinator& coord, float mass,
                      std::unique_ptr<btCollisionShape> shape);

    // Supprime le rigid body associe a une entite
    void RemoveRigidBody(Entity entity);

    // Avance la simulation physique et synchronise les Transforms
    void Update(Coordinator& coord, float dt);

    // Libere toutes les ressources Bullet
    void Shutdown();

    // Verifie si une entite a deja un rigid body
    bool HasRigidBody(Entity entity) const;

private:
    // Composants du monde Bullet
    std::unique_ptr<btDefaultCollisionConfiguration>     m_collisionConfig;
    std::unique_ptr<btCollisionDispatcher>               m_dispatcher;
    std::unique_ptr<btDbvtBroadphase>                    m_broadphase;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld>             m_dynamicsWorld;

    // Association Entity -> btRigidBody (+ collision shape + motion state)
    struct BodyData
    {
        std::unique_ptr<btRigidBody>          rigidBody;
        std::unique_ptr<btCollisionShape>     shape;
        std::unique_ptr<btDefaultMotionState> motionState;
    };

    std::unordered_map<Entity, BodyData> m_bodies;
};