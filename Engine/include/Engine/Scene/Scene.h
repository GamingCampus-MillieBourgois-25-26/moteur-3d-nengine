#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/ECS/Systems/RenderSystem.h"
#include "Engine/Renderer.h"
#include "Engine/ScriptManager.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/MeshRenderer.h"
#include "Engine/ECS/Components/Name.h"
#include "Engine/ECS/Components/Script.h"

class Scene
{
public:
    Scene(const std::string& name, Renderer* renderer, ScriptManager* scriptManager);

    const std::string& GetName() const { return m_name; }

    Entity CreateRenderableEntity();

    void DestroyEntity(Entity e);

    const std::vector<Entity>& GetEntities() const { return m_entities; }

    template<typename T>
    void AddComponent(Entity e, const T& c) { m_coordinator.AddComponent<T>(e, c); }

    template<typename T>
    T& GetComponent(Entity e) { return m_coordinator.GetComponent<T>(e); }

    Transform GetTransform(Entity e);
    void SetTransform(Entity e, const Transform& t);

    void Update(float dt);

    void Render();

private:
    std::string m_name;
    Renderer* m_renderer = nullptr;
    ScriptManager* m_scriptManager = nullptr;

    Coordinator m_coordinator;
    std::vector<Entity> m_entities;

    std::shared_ptr<RenderSystem> m_renderSystem;
};
