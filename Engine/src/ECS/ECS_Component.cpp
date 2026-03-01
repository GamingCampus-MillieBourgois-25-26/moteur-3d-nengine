#include "Engine/ECS/ECS_Component.h"

void ComponentManager::EntityDestroyed(Entity entity)
{
    for (auto const& pair : mComponentArrays)
    {
        auto const& componentArray = pair.second;
        componentArray->EntityDestroyed(entity);
    }
}