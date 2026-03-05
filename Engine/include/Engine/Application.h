#pragma once
#include <windows.h>
#include <iostream>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

#include "Window.h"
#include "AudioSystem.h"
#include "OBJ/OBJLoader.h"
#include "Renderer.h"
#include "ScriptManager.h"
#include "Input.h"

#include "Engine/Scene/SceneManager.h"

#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/MeshRenderer.h"

#include <chrono>
#include <filesystem>
#include <string>
#include <vector>





namespace Engine {

    class Application
    {
    public:
        Application() {};
        ~Application() {};

        void Init();
        void Update(float dt);
        void Shutdown();

        Renderer& GetRenderer() { return renderer; }
        WindowInstance& GetWindow() { return window; }

        bool getIsRunning() const { return isRunning; }
        void setIsRunning(bool v) { isRunning = v; }

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

    private:
        bool isRunning = true;


        SceneManager m_sceneManager;

        AudioSystem audio;
        ScriptManager scriptManager;
        Renderer renderer;
       /* Transform tr;
        MeshRenderer mr;
        Velocity vel;
        Name name;
        Script script;*/

        OBJLoader loader;
        WindowInstance window;
        std::unique_ptr<Input> input;

        using clock = std::chrono::high_resolution_clock;

        float mouseSensitivity = 0.002f;
        float speed = 0.f;

        // list of entities created via the editor/runtime (keeps ordering for UI)
        std::vector<::Entity> m_sceneEntities;
    };

} // namespace Engine
