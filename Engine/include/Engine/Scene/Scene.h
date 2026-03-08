#pragma once
/**
 * @file Scene.h
 * @brief A self-contained game scene with its own ECS, physics and render pipeline.
 * @ingroup Engine
 */

#include <vector>
#include <memory>
#include <string>
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/Renderer.h"
#include "Engine/ScriptManager.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/MeshRenderer.h"
#include "Engine/ECS/Components/Name.h"
#include "Engine/ECS/Components/Script.h"
#include "Engine/OBJ/NewOBJLoader.h"
#include "Engine/PhysicsIncluder.h"

/**
 * @brief Encapsulates a complete game scene.
 *
 * Each Scene owns:
 *  - An ECS Coordinator (entities + components + systems).
 *  - A PhysicCustom bundle (all physics sub-systems).
 *  - Shared pointers to the active RenderSystem and MovementSystem.
 *  - A list of living entities for editor enumeration.
 *
 * The SceneManager creates and activates scenes; only one scene is active
 * at a time.  The active scene's Update() and Render() are driven by
 * Application::Running().
 */
class Scene
{
public:
    /**
     * @brief Constructs a Scene with all required sub-systems.
     * @param name          Human-readable scene name.
     * @param renderer      Non-owning pointer to the shared Renderer.
     * @param scriptManager Non-owning pointer to the shared ScriptManager.
     */
    Scene(const std::string& name, Renderer* renderer, ScriptManager* scriptManager);

    /// @brief Returns the scene name.
    const std::string& GetName() const { return m_name; }

    /**
     * @brief Creates a renderable entity (Transform + MeshRenderer + Name).
     * @return ID of the new entity.
     */
    Entity CreateRenderableEntity();

    /**
     * @brief Destroys @p e and removes all its components.
     * @param e  Entity to destroy.
     */
    void DestroyEntity(Entity e);

    /// @brief Returns the list of all living entities in this scene.
    const std::vector<Entity>& GetEntities() const { return m_entities; }

    /**
     * @brief Adds component @p c of type T to @p e.
     * @tparam T  Component type.
     */
    template<typename T>
    void AddComponent(Entity e, const T& c) { m_coordinator.AddComponent<T>(e, c); }

    /**
     * @brief Returns a reference to the T component of @p e.
     * @tparam T  Component type.
     */
    template<typename T>
    T& GetComponent(Entity e) { return m_coordinator.GetComponent<T>(e); }

    /**
     * @brief Returns a copy of the Transform component of @p e.
     * @param e  Target entity.
     */
    Transform GetTransform(Entity e);

    /**
     * @brief Overwrites the Transform component of @p e.
     * @param e  Target entity.
     * @param t  New transform value.
     */
    void SetTransform(Entity e, const Transform& t);

    /**
     * @brief Updates all systems (physics, movement, scripts).
     * @param dt  Delta-time in seconds.
     */
    void Update(float dt);

    /**
     * @brief Submits all renderable entities to the Renderer.
     */
    void Render();

private:
    std::string       m_name;           ///< Scene identifier.
    Renderer*         m_renderer      = nullptr; ///< Shared renderer (non-owning).
    ScriptManager*    m_scriptManager = nullptr; ///< Shared script manager (non-owning).
    Coordinator       m_coordinator;    ///< Scene-local ECS coordinator.
    std::vector<Entity> m_entities;     ///< Ordered list of living entities.
    PhysicCustom        customPH;       ///< All physics sub-systems.
    std::shared_ptr<RenderSystem>   m_renderSystem;   ///< Render system instance.
    std::shared_ptr<MovementSystem> m_movementSystem; ///< Movement system instance.
};
