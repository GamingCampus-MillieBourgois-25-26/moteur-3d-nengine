#pragma once
#include "Engine/ECS/ECS_Types.h"

class IComponentArray // Le ComponentManager doit pouvoir appeler EntityDestroyed() sans connaître le type T.
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};