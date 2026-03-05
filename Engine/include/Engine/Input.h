#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <array>

/**
 * @brief Interface abstraite pour les backends d'entrée.
 *
 * Permet d'implémenter différents systèmes d'entrée
 * (GLFW, SDL, etc.) de manière interchangeable.
 */
class IInputBackend
{
public:
    virtual ~IInputBackend() = default;

    /** @brief Récupère les événements d'entrée du backend. */
    virtual void PollEvents() = 0;

    /**
     * @brief Indique si une touche est actuellement enfoncée.
     * @param key Code de la touche
     * @return true si la touche est enfoncée
     */
    virtual bool IsKeyDown(int key) const = 0;

    /**
     * @brief Indique si un bouton de souris est enfoncé.
     * @param button Index du bouton
     * @return true si le bouton est enfoncé
     */
    virtual bool IsMouseButtonDown(int button) const = 0;

    /**
     * @brief Récupère la position actuelle de la souris.
     * @param x Position X
     * @param y Position Y
     */
    virtual void GetMousePosition(double& x, double& y) const = 0;

    /**
     * @brief Consomme le déplacement horizontal de la molette.
     * @return Valeur du scroll horizontal
     */
    virtual float ConsumeScrollX() = 0;

    /**
     * @brief Consomme le déplacement vertical de la molette.
     * @return Valeur du scroll vertical
     */
    virtual float ConsumeScrollY() = 0;
};

/**
 * @brief Système de gestion des entrées utilisateur.
 *
 * Gère les actions, axes, contextes et états des périphériques
 * via un backend d'entrée abstrait.
 */
class Input
{
public:
    /**
     * @brief Construit le système d'entrée avec un backend donné.
     * @param backend Backend d'entrée utilisé
     */
    explicit Input(std::unique_ptr<IInputBackend> backend);

    ~Input() = default;

    /** @brief Met à jour l'état des entrées. */
    void Update();

    /**
     * @brief Type de binding d'entrée.
     */
    enum class BindingType { Action, Axis };

    /**
     * @brief Représente une liaison entre une entrée et une action ou un axe.
     */
    struct Binding
    {
        int key;
        std::string name;
        BindingType type;
        float scale = 1.f;
        bool consume = true;
    };

    /**
     * @brief Contexte d'entrée regroupant des bindings.
     *
     * Les contextes peuvent être empilés pour gérer
     * différentes couches d'entrée.
     */
    class Context
    {
    public:
        /**
         * @brief Associe une touche à une action.
         * @param key Code de la touche
         * @param name Nom de l'action
         * @param consume Indique si l'entrée est consommée
         */
        void BindAction(int key, const std::string& name, bool consume = true);

        /**
         * @brief Associe une touche à un axe.
         * @param key Code de la touche
         * @param name Nom de l'axe
         * @param scale Facteur de l'axe
         */
        void BindAxis(int key, const std::string& name, float scale);

        /**
         * @brief Modifie la touche associée à un binding existant.
         * @param name Nom du binding
         * @param newKey Nouvelle touche
         */
        void Rebind(const std::string& name, int newKey);

        /**
         * @brief Structure sérialisable d'un binding.
         */
        struct SerializableBinding
        {
            std::string name;
            int key;
            BindingType type;
            float scale;
            bool consume;
        };

        /**
         * @brief Sérialise les bindings du contexte.
         * @return Liste de bindings sérialisés
         */
        std::vector<SerializableBinding> Serialize() const;

        /**
         * @brief Désérialise des bindings dans le contexte.
         * @param data Données sérialisées
         */
        void Deserialize(const std::vector<SerializableBinding>& data);

    private:
        friend class Input;
        std::vector<Binding> m_bindings;
    };

    /**
     * @brief Crée un nouveau contexte d'entrée.
     * @return Contexte créé
     */
    std::shared_ptr<Context> CreateContext();

    /**
     * @brief Empile un contexte d'entrée.
     * @param ctx Contexte à empiler
     */
    void PushContext(const std::shared_ptr<Context>& ctx);

    /** @brief Retire le contexte d'entrée actif. */
    void PopContext();

    /**
     * @brief Indique si une action est active.
     * @param name Nom de l'action
     * @return true si l'action est active
     */
    bool Action(const std::string& name) const;

    /**
     * @brief Indique si une action vient d'être pressée.
     * @param name Nom de l'action
     * @return true si l'action vient d'être pressée
     */
    bool ActionPressed(const std::string& name) const;

    /**
     * @brief Indique si une action vient d'être relâchée.
     * @param name Nom de l'action
     * @return true si l'action vient d'être relâchée
     */
    bool ActionReleased(const std::string& name) const;

    /**
     * @brief Récupère la valeur d'un axe.
     * @param name Nom de l'axe
     * @return Valeur de l'axe
     */
    float Axis(const std::string& name) const;

    /** @brief Déplacement horizontal de la souris. */
    float MouseDX() const { return m_mouseDX; }

    /** @brief Déplacement vertical de la souris. */
    float MouseDY() const { return m_mouseDY; }

    /** @brief Défilement horizontal de la molette. */
    float ScrollX() const { return m_scrollX; }

    /** @brief Défilement vertical de la molette. */
    float ScrollY() const { return m_scrollY; }

    /**
     * @brief Récupère une touche actuellement pressée.
     * @return Code de la touche
     */
    int GetAnyPressedKey() const;

    static constexpr int MOUSE_OFFSET = 1000;
    static constexpr int GAMEPAD_OFFSET = 2000;

private:
    enum class KeyState { Up, Pressed, Down, Released };

    void PollKeyboard();
    void PollMouseButtons();
    void PollMouse();
    void ResolveContexts();

    std::unique_ptr<IInputBackend> m_backend;

    static constexpr int KEY_COUNT = 349;
    static constexpr int MOUSE_BUTTON_COUNT = 8;

    std::array<KeyState, KEY_COUNT> m_keys;
    std::array<KeyState, MOUSE_BUTTON_COUNT> m_mouseButtons;

    std::unordered_map<std::string, bool> m_actionDown;
    std::unordered_map<std::string, bool> m_actionPressed;
    std::unordered_map<std::string, bool> m_actionReleased;
    std::unordered_map<std::string, float> m_axes;

    std::vector<std::shared_ptr<Context>> m_contexts;

    double m_lastMouseX = 0.0;
    double m_lastMouseY = 0.0;
    float m_mouseDX = 0.f;
    float m_mouseDY = 0.f;
    bool m_firstMouse = true;

    float m_scrollX = 0.f;
    float m_scrollY = 0.f;
};

/**
 * @brief Crée un système d'entrée basé sur GLFW.
 * @param window Fenêtre GLFW associée
 * @return Instance du système d'entrée
 */
struct GLFWwindow;
std::unique_ptr<Input> CreateGLFWInput(GLFWwindow* window);
