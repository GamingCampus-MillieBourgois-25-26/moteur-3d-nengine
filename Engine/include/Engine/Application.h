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

#include "Input.h"
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
    private:
        bool isRunning = false;

        Renderer renderer;
        AudioSystem audio;
        OBJLoader loader;
        WindowInstance window;
        std::unique_ptr<Input> input;
        // ECS 

        Coordinator coord;
        std::shared_ptr<RenderSystem> renderSystem;
        std::shared_ptr<MovementSystem> movementSystem;

        using clock = std::chrono::high_resolution_clock;

        float mouseSensitivity = 0.002f;
        float speed = 0.f;

        // list of entities created via the editor/runtime (keeps ordering for UI)
        std::vector<::Entity> m_sceneEntities;

    public:

        // Initialise le moteur (n'entre plus dans la boucle)
        void Init();

        // Exécute une itération (appelé par l'exécutable qui gère ImGui)
        void Update(float dt);

        void Shutdown();


        bool getIsRunning() const { return isRunning; }
        void setIsRunning(bool running) { isRunning = running; }

        // Accesseurs pour l'éditeur / ImGui
        WindowInstance& GetWindow() { return window; }
        Renderer& GetRenderer() { return renderer; }
        Input* GetInput() const { return input.get(); }

        // --- Editor-friendly ECS helpers ---
        // Crée une entité, lui ajoute Transform + MeshRenderer (utilise le mesh courant du renderer)
        ::Entity CreateRenderableEntity();

        // Retourne la liste des entités créées (par l'éditeur / runtime)
        const std::vector<::Entity>& GetEntities() const { return m_sceneEntities; }

        // Get / Set Transform for an entity (copies)
        ::Transform GetTransform(::Entity entity);
        void SetTransform(::Entity entity, const ::Transform& t);
    };
}