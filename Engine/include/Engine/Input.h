#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

struct GLFWwindow;

// ===== Input =====
class Input
{
public:
    explicit Input(GLFWwindow* window);

    void Update();

    // Contexts
    struct Binding
    {
        int key;
        std::string action;
        float scale;
    };

    struct Context
    {
        std::vector<Binding> bindings;
        void Bind(int key, const std::string& action, float scale = 1.f)
        {
            bindings.push_back({ key, action, scale });
        }
    };

    void PushContext(Context* ctx);
    void PopContext();

    // Gameplay API
    bool Action(const std::string& name) const;
    float Axis(const std::string& name) const;

    // Mouse
    float MouseDX() const;
    float MouseDY() const;

private:
    enum class KeyState { Up, Pressed, Down, Released };

    void PollKeyboard();
    void PollMouse();

    GLFWwindow* m_window = nullptr;

    std::unordered_map<int, KeyState> m_keys;
    std::unordered_map<std::string, float> m_axes;
    std::unordered_map<std::string, bool> m_actions;

    std::vector<Context*> m_contexts;

    double m_lastMouseX = 0.0;
    double m_lastMouseY = 0.0;
    float m_mouseDX = 0.f;
    float m_mouseDY = 0.f;
    bool m_firstMouse = true;
};
