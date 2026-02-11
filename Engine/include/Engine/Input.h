#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

// Interface abstraite pour le backend
class IInputBackend
{
public:
    virtual ~IInputBackend() = default;

    virtual void PollEvents() = 0;
    virtual bool IsKeyDown(int key) const = 0;
    virtual void GetMousePosition(double& x, double& y) const = 0;
};

class Input
{
public:
    explicit Input(std::unique_ptr<IInputBackend> backend);
    ~Input() = default;

    void Update();

    // ========================
    // Context / Bindings
    // ========================

    enum class BindingType
    {
        Action,
        Axis
    };

    struct Binding
    {
        int key;
        std::string name;
        BindingType type;
        float scale = 1.f;
        bool consume = true;
    };

    class Context
    {
    public:
        void BindAction(int key, const std::string& name, bool consume = true);
        void BindAxis(int key, const std::string& name, float scale);

    private:
        friend class Input;
        std::vector<Binding> m_bindings;
    };

    std::shared_ptr<Context> CreateContext();
    void PushContext(const std::shared_ptr<Context>& ctx);
    void PopContext();

    // ========================
    // Gameplay API
    // ========================

    bool Action(const std::string& name) const;
    bool ActionPressed(const std::string& name) const;
    bool ActionReleased(const std::string& name) const;
    float Axis(const std::string& name) const;

    float MouseDX() const { return m_mouseDX; }
    float MouseDY() const { return m_mouseDY; }

private:
    enum class KeyState { Up, Pressed, Down, Released };

    void PollKeyboard();
    void PollMouse();
    void ResolveContexts();

    std::unique_ptr<IInputBackend> m_backend;

    std::unordered_map<int, KeyState> m_keys;

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
};
