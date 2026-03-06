#pragma once
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Systems/PhysicsBodySystem.h"
#include "ECS/Systems/ColliderSystem.h"
#include "ECS/Systems/TriggerSystem.h"
#include "ECS/Systems/ForceSystem.h"
#include "ECS/Systems/JointSystem.h"
#include "ECS/Systems/CharacterControllerSystem.h"

struct PhysicCustom {
	// Physics
	std::shared_ptr<PhysicsBodySystem> physicsBodySystem;
	std::shared_ptr<ColliderSystem> colliderSystem;
	std::shared_ptr<TriggerSystem> triggerSystem;
	std::shared_ptr<ForceSystem> forceSystem;
	std::shared_ptr<JointSystem> jointSystem;
	std::shared_ptr<CharacterControllerSystem> characterControllerSystem;
};