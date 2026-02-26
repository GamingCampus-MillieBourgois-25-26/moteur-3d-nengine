#include "Engine/ECS/Systems/MovementSystem.h"


void MovementSystem::Update(Coordinator& coord)
{
    for (auto entity : mEntities)
    {
        auto& t = coord.GetComponent<Transform>(entity);
        auto& v = coord.GetComponent<Velocity>(entity);

        t.position = t.position + v.velocity;
    }
}