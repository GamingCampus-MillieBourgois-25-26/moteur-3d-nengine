#include "Engine/ECS/Systems/MovementSystem.h"

void MovementSystem::Update(Coordinator& coord, float dt)
{
    // mEntities contient toutes les entités ayant Transform + Velocity
    for (auto entity : mEntities) 
    {
        // Récupération des composants nécessaires
        auto& t = coord.GetComponent<Transform>(entity);
        auto& v = coord.GetComponent<Velocity>(entity);

        // Déplacement : position += velocity * dt
        t.position = t.position + v.velocity * dt; 

        // Rotation autour de l’axe X
        t.rotation = t.rotation * MathsLib::Quaternion<float>::Euler(rotationSpeedX * dt, 0, 0);
        // Rotation autour de l’axe Y
        t.rotation = t.rotation * MathsLib::Quaternion<float>::Euler(0, rotationSpeedY * dt, 0);
        // Rotation autour de l’axe Z
        t.rotation = t.rotation * MathsLib::Quaternion<float>::Euler(0, 0, rotationSpeedZ * dt);

        // Normalisation obligatoire pour éviter la dérive du quaternion
        t.rotation.Normalize();
    }
}