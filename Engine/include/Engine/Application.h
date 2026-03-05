#pragma once
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
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Systems/PhysicsBodySystem.h"
#include "ECS/Systems/ColliderSystem.h"
#include "ECS/Systems/TriggerSystem.h"
#include "ECS/Systems/ForceSystem.h"
#include "ECS/Systems/JointSystem.h"
#include "ECS/Systems/CharacterControllerSystem.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/ECS/ECS_Coordinator.h"

#include <chrono>
#include <filesystem>
#include <string>
#include <vector>

namespace Engine {

    class Application
    {
    private:
        bool isRunning = true;


        SceneManager m_sceneManager;

        AudioSystem audio;
        WindowInstance window;
        Coordinator coord;
        std::unique_ptr<Input> input;
        std::shared_ptr<RenderSystem> renderSystem;
        std::shared_ptr<MovementSystem> movementSystem;
        std::shared_ptr<PhysicsBodySystem> physicsBodySystem;
		std::shared_ptr<ColliderSystem> colliderSystem;
		std::shared_ptr<TriggerSystem> triggerSystem;
		std::shared_ptr<ForceSystem> forceSystem;
		std::shared_ptr<JointSystem> jointSystem;
		std::shared_ptr<CharacterControllerSystem> characterControllerSystem;
        ScriptManager scriptManager;
        Renderer renderer;

        using clock = std::chrono::high_resolution_clock;

        float mouseSensitivity = 0.002f;
        float speed = 0.f;

    public:
        Application() {};
        ~Application() {};

        void Init();
        void Update(float dt);
        void Running();
        void Shutdown();

        Renderer& GetRenderer() { return renderer; }
        WindowInstance& GetWindow() { return window; }

        bool getIsRunning() const { return isRunning; }
        void setIsRunning(bool running) { isRunning = running; }

        // ---------- Façade pour le main / éditeur ----------

        Entity CreateRenderableEntity();
        void DestroyEntity(Entity e);

        const std::vector<Entity>& GetEntities() const;

        template<typename T>
        void AddComponent(Entity e, const T& c)
        {
            Scene* scene = m_sceneManager.GetActiveScene();
            if (scene)
                scene->AddComponent<T>(e, c);
        }

        template<typename T>
        T& GetComponent(Entity e)
        {
            Scene* scene = m_sceneManager.GetActiveScene();
            // copie par valeur pour rester simple
            return scene->GetComponent<T>(e);
        }

        Transform GetTransform(Entity e);
        void SetTransform(Entity e, const Transform& t);

        // list of entities created via the editor/runtime (keeps ordering for UI)
        std::vector<::Entity> m_sceneEntities;
    };
}
