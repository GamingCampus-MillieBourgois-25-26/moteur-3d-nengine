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
#include <chrono>
#include <filesystem>
#include <string>
#include <vector>

namespace Engine {

    /**
     * @brief Classe principale de l'application moteur.
     *
     * Gère l'initialisation, la boucle principale et l'arrêt
     * des différents systèmes du moteur.
     */
    class Application
    {
    private:
        bool isRunning = false;

        Renderer renderer;
        AudioSystem audio;
        //OBJLoader loader;
        WindowInstance window;
        std::unique_ptr<Input> input;

        Coordinator coord;
        std::shared_ptr<RenderSystem> renderSystem;
        std::shared_ptr<MovementSystem> movementSystem;

        using clock = std::chrono::high_resolution_clock;

        float mouseSensitivity = 0.002f;
        float speed = 0.f;

    public:

        /**
         * @brief Initialise l'application et ses systèmes.
         */
        void Init();

        /**
         * @brief Lance la boucle principale de l'application.
         */
        void Running();

        /**
         * @brief Arrête l'application et libère les ressources.
         */
        void Shutdown();

        /**
         * @brief Indique si l'application est en cours d'exécution.
         * @return true si l'application est active, false sinon
         */
        bool getIsRunning() const { return isRunning; }

        /**
         * @brief Définit l'état d'exécution de l'application.
         * @param running Nouvel état d'exécution
         */
        void setIsRunning(bool running) { isRunning = running; }
    };
}
