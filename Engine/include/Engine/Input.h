#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <array>

// ========================
// Backend Interface
// ========================

// Interface abstraite pour la source des inputs (GLFW ici)
class IInputBackend
{
public:
    virtual ~IInputBackend() = default;

    virtual void PollEvents() = 0;
    virtual bool IsKeyDown(int key) const = 0;
    virtual bool IsMouseButtonDown(int button) const = 0;
    virtual void GetMousePosition(double& x, double& y) const = 0;

    // Scroll consommé une fois par frame
    virtual float ConsumeScrollX() = 0;
    virtual float ConsumeScrollY() = 0;
};

// ========================
// Input System
// ========================

class Input
{
public:
    explicit Input(std::unique_ptr<IInputBackend> backend);
    ~Input() = default;

    // Mise à jour par frame
    void Update();

    // Type de binding
    enum class BindingType { Action, Axis };

    // Association touche -> action / axe
    struct Binding
    {
        int key;
        std::string name;
        BindingType type;
        float scale = 1.f;
        bool consume = true;
    };

    // ========================
    // Context
    // ========================

    class Context
    {
    public:
        // Ajoute une nouvelle action
        // Exemple :
        // ctx.BindAction(GLFW_KEY_E, "Interact");
        void BindAction(int key, const std::string& name, bool consume = true);

        // Ajoute un nouvel axe
        // Exemple :
        // ctx.BindAxis(GLFW_KEY_W, "MoveForward", +1.f);
        // ctx.BindAxis(GLFW_KEY_S, "MoveForward", -1.f);
        void BindAxis(int key, const std::string& name, float scale);

        // Change la touche associée à une action existante
        void Rebind(const std::string& name, int newKey);

        // Structure utilisée pour la sérialisation JSON
        struct SerializableBinding
        {
            std::string name;
            int key;
            BindingType type;
            float scale;
            bool consume;
        };

        std::vector<SerializableBinding> Serialize() const;
        void Deserialize(const std::vector<SerializableBinding>& data);

    private:
        friend class Input;
        std::vector<Binding> m_bindings;
    };

    // Création et gestion des contextes
    std::shared_ptr<Context> CreateContext();
    void PushContext(const std::shared_ptr<Context>& ctx);
    void PopContext();

    // ========================
    // API Gameplay
    // ========================

    bool Action(const std::string& name) const;
    bool ActionPressed(const std::string& name) const;
    bool ActionReleased(const std::string& name) const;
    float Axis(const std::string& name) const;

    float MouseDX() const { return m_mouseDX; }
    float MouseDY() const { return m_mouseDY; }
    float ScrollX() const { return m_scrollX; }
    float ScrollY() const { return m_scrollY; }

    // Utilisé pour le rebinding
    int GetAnyPressedKey() const;

    // Offsets pour différencier les périphériques
    static constexpr int MOUSE_OFFSET = 1000;

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

    // États logiques exposés au gameplay
    std::unordered_map<std::string, bool> m_actionDown;
    std::unordered_map<std::string, bool> m_actionPressed;
    std::unordered_map<std::string, bool> m_actionReleased;
    std::unordered_map<std::string, float> m_axes;

    // Pile de contextes
    std::vector<std::shared_ptr<Context>> m_contexts;

    // Données souris
    double m_lastMouseX = 0.0;
    double m_lastMouseY = 0.0;
    float m_mouseDX = 0.f;
    float m_mouseDY = 0.f;
    bool m_firstMouse = true;

    float m_scrollX = 0.f;
    float m_scrollY = 0.f;
};
