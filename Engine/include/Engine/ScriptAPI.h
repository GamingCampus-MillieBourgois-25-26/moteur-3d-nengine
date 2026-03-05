#pragma once

/**
 * @brief Contexte passé aux scripts lors de leur mise à jour.
 */
struct ScriptContext {
    float dt; /**< Temps écoulé depuis la dernière mise à jour */
};

/**
 * @brief Interface de base pour les scripts.
 *
 * Définit le cycle de vie standard d'un script :
 * création, mise à jour et destruction.
 */
class IScript {
public:
    virtual ~IScript() = default;

    /** @brief Appelé lors de la création du script. */
    virtual void OnCreate() {}

    /**
     * @brief Appelé à chaque mise à jour du script.
     * @param ctx Contexte de mise à jour
     */
    virtual void OnUpdate(const ScriptContext& ctx) {}

    /** @brief Appelé lors de la destruction du script. */
    virtual void OnDestroy() {}
};
