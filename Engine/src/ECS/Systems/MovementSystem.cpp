#include "Engine/ECS/Systems/MovementSystem.h"

void MovementSystem::Update(Coordinator& coord, float dt)
{
    for (auto entity : mEntities) // pour entre ici il faut que l'entity ai un transform et une velocity
    {
        auto& t = coord.GetComponent<Transform>(entity);
        auto& v = coord.GetComponent<Velocity>(entity);

        // Déplacement
        t.position = t.position + v.velocity * dt; 

        // Rotation sur l’axe X
        t.rotation = t.rotation * MathsLib::Quaternion<float>::Euler(rotationSpeedX * dt, 0, 0);
        // Rotation sur l’axe Y
        t.rotation = t.rotation * MathsLib::Quaternion<float>::Euler(0, rotationSpeedY * dt, 0);
        // Rotation sur l’axe Z
        t.rotation = t.rotation * MathsLib::Quaternion<float>::Euler(0, 0, rotationSpeedZ * dt);

        // Important : normaliser le quaternion
        t.rotation.Normalize();
        std::cout << "feur\n";
    }
}