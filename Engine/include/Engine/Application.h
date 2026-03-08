#pragma once
/**
 * @file Application.h
 * @brief Top-level engine application – owns all subsystems and drives the main loop.
 * @ingroup Engine
 */

#include <windows.h>
#include <iostream>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include "Window.h"
#include "AudioSystem.h"
#include "Renderer.h"
#include "ScriptManager.h"
#include "Input.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/ECS/ECS_Coordinator.h"
#include "Engine/PhysicsIncluder.h"
#include <chrono>
#include <filesystem>
#include <string>
#include <vector>

namespace Engine
{
    /**
     * @brief Central engine object that owns and orchestrates every subsystem.
     *
     * Application is the entry point for both the editor and the runtime.
     * It creates the window, initialises the renderer, audio, physics and
     * scripting systems, then runs the main loop.
     *
     * Typical usage (minimal runtime):
     * @code
     *  Engine::Application app;
     *  app.Init();
     *  app.Running();   // blocks until the window is closed
     *  app.Shutdown();
     * @endcode
     *
     * The class also exposes a thin facade so the editor can manipulate
     * entities without reaching into the scene directly.
     */
    class Application
    {
    private:
        bool         isRunning      = true;  ///< Main-loop condition flag.
        SceneManager m_sceneManager;          ///< Scene collection and activation.
        AudioSystem  audio;                   ///< FMOD Studio audio backend.
        WindowInstance window;                ///< GLFW window wrapper.
        Coordinator  coord;                   ///< Root ECS coordinator (pre-scene).
        std::unique_ptr<Input> input;         ///< High-level input system.
        std::shared_ptr<RenderSystem>   renderSystem;   ///< ECS render system.
        std::shared_ptr<MovementSystem> movementSystem; ///< ECS movement system.
        PhysicCustom  customP;                ///< All physics sub-systems.
        ScriptManager scriptManager;          ///< Gameplay script loader.
        Renderer      renderer;               ///< D3D11 renderer.

        using clock = std::chrono::high_resolution_clock;

        float mouseSensitivity = 0.002f; ///< Mouse look sensitivity (radians/pixel).
        float speed            = 0.f;    ///< Camera movement speed (m/s).

    public:
        Application()  {};
        ~Application() {};

        // ── Lifecycle ─────────────────────────────────────────────────────

        /**
         * @brief Initialises all subsystems (window, renderer, audio, physics, input).
         * Must be called once before Running().
         */
        void Init();

        /**
         * @brief Performs a single-frame update (physics, scripts, input).
         * @param dt  Delta-time in seconds since the last frame.
         */
        void Update(float dt);

        /**
         * @brief Runs the main loop until the window is closed or setIsRunning(false).
         *
         * Internally calls Update() and Render() each iteration while
         * measuring delta-time with a high-resolution clock.
         */
        void Running();

        /**
         * @brief Tears down all subsystems in reverse initialisation order.
         */
        void Shutdown();

        // ── Accessors ─────────────────────────────────────────────────────

        /// @brief Returns a reference to the active Renderer.
        Renderer&        GetRenderer() { return renderer; }

        /// @brief Returns a reference to the GLFW window wrapper.
        WindowInstance&  GetWindow()   { return window; }

        /// @brief Returns true while the main loop should continue.
        bool getIsRunning() const      { return isRunning; }

        /**
         * @brief Sets the running flag (pass false to request a clean shutdown).
         * @param running  New running state.
         */
        void setIsRunning(bool running) { isRunning = running; }

        // ── Editor / runtime entity facade ────────────────────────────────

        /**
         * @brief Creates a renderable entity in the active scene.
         * @return ID of the new entity.
         */
        Entity CreateRenderableEntity();

        /**
         * @brief Destroys @p e in the active scene.
         * @param e  Entity to destroy.
         */
        void DestroyEntity(Entity e);

        /**
         * @brief Returns all entities currently in the active scene.
         */
        const std::vector<Entity>& GetEntities() const;

        /**
         * @brief Adds component @p c of type T to @p e in the active scene.
         * @tparam T  Component type.
         * @param e   Target entity.
         * @param c   Component value to attach.
         */
        template<typename T>
        void AddComponent(Entity e, const T& c)
        {
            Scene* scene = m_sceneManager.GetActiveScene();
            if (scene)
                scene->AddComponent<T>(e, c);
        }

        /**
         * @brief Returns a reference to the T component of @p e in the active scene.
         * @tparam T  Component type.
         * @param e   Target entity.
         */
        template<typename T>
        T& GetComponent(Entity e)
        {
            Scene* scene = m_sceneManager.GetActiveScene();
            return scene->GetComponent<T>(e);
        }

        /**
         * @brief Returns a copy of the Transform component of @p e.
         * @param e  Target entity.
         */
        Transform GetTransform(Entity e);

        /**
         * @brief Overwrites the Transform of @p e.
         * @param e  Target entity.
         * @param t  New transform value.
         */
        void SetTransform(Entity e, const Transform& t);

        /// @brief Ordered list of entities created via the editor or runtime.
        std::vector<::Entity> m_sceneEntities;
    };
} // namespace Engine
