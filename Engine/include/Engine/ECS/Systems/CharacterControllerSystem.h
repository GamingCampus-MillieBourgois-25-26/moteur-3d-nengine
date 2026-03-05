#pragma once

#include "Engine/ECS/ECS_System.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Components/CharacterController.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Systems/PhysicsBodySystem.h"

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <memory>
#include <unordered_map>

/*
    CharacterControllerSystem
    -------------------------
    Systeme responsable de gerer les personnages jouables via
    btKinematicCharacterController de Bullet.

    Responsabilites :
    - Detecter les entites ayant Transform + CharacterController non initialise
    - Creer le btPairCachingGhostObject + btCapsuleShape + btKinematicCharacterController
    - Appliquer la direction de deplacement et le saut chaque frame
    - Synchroniser la position du ghost object vers le composant Transform

    Le PhysicsBodySystem reste proprietaire du monde Bullet.
    Ce systeme cree et gere les character controllers.

    Ce systeme traite les entites ayant :
    - Transform
    - CharacterController
*/

class CharacterControllerSystem : public System
{
public:
    // Associe le PhysicsBodySystem pour acceder au monde Bullet
    void Init(std::shared_ptr<PhysicsBodySystem> physicsSystem);

    // Parcourt les entites, cree les controllers non initialises,
    // applique les deplacements et synchronise les Transforms
    void Update(Coordinator& coord, float dt);

    // Supprime le character controller associe a une entite
    void RemoveController(Entity entity);

    // Libere toutes les ressources
    void Shutdown();

private:
    // Cree le btKinematicCharacterController pour une entite
    void CreateController(Entity entity, Coordinator& coord);

    // Reference vers le PhysicsBodySystem (proprietaire du monde Bullet)
    std::shared_ptr<PhysicsBodySystem> m_physicsSystem;

    // Donnees internes par entite
    struct ControllerData
    {
        std::unique_ptr<btKinematicCharacterController> controller;
        std::unique_ptr<btPairCachingGhostObject>       ghostObject;
        std::unique_ptr<btCapsuleShape>                 shape;
    };

    std::unordered_map<Entity, ControllerData> m_controllers;
};
