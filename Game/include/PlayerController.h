#pragma once
#include <Engine/ScriptAPI.h>
#include <iostream>

/**
 * @brief Script de contrôle du joueur.
 *
 * Implémente la logique de base du joueur
 * via le système de scripts du moteur.
 */
class PlayerController : public IScript {
public:
    /**
     * @brief Appelé lors de la création du script.
     */
    void OnCreate() override {
        std::cout << "PlayerController created\n";
    }

    /**
     * @brief Appelé à chaque mise à jour du script.
     * @param ctx Contexte de mise à jour
     */
    void OnUpdate(const ScriptContext& ctx) override {
        std::cout << "PlayerController update dt=" << ctx.dt << "\n";
    }

    /**
     * @brief Appelé lors de la destruction du script.
     */
    void OnDestroy() override {
        std::cout << "PlayerController destroyed\n";
    }
};
