#include "Engine/Input.h"
#include <GLFW/glfw3.h>
#include <iostream>

// ========================
// Backend GLFW concret
// ========================

class GLFWBackend : public IInputBackend
{
public:
    explicit GLFWBackend(GLFWwindow* window)
        : m_window(window)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void PollEvents() override { glfwPollEvents(); }

    bool IsKeyDown(int key) const override
    {
        return glfwGetKey(m_window, key) == GLFW_PRESS;
    }

    void GetMousePosition(double& x, double& y) const override
    {
        glfwGetCursorPos(m_window, &x, &y);
    }

private:
    GLFWwindow* m_window;
};

// ========================
// Input Implementation
// ========================

Input::Input(std::unique_ptr<IInputBackend> backend)
    : m_backend(std::move(backend))
{
    for (int key = 0; key <= 348; ++key) // GLFW_KEY_LAST = 348
        m_keys[key] = KeyState::Up;
}

void Input::Update()
{
    m_backend->PollEvents();

    PollKeyboard();
    PollMouse();

    m_actionDown.clear();
    m_actionPressed.clear();
    m_actionReleased.clear();
    m_axes.clear();

    ResolveContexts();
}

// ------------------------
// Keyboard polling
// ------------------------

void Input::PollKeyboard()
{
    for (auto& [key, ks] : m_keys)
    {
        bool pressed = m_backend->IsKeyDown(key);

        if (pressed)
        {
            ks = (ks == KeyState::Up || ks == KeyState::Released)
                ? KeyState::Pressed
                : KeyState::Down;
        }
        else
        {
            ks = (ks == KeyState::Down || ks == KeyState::Pressed)
                ? KeyState::Released
                : KeyState::Up;
        }
    }
}

// ------------------------
// Mouse polling
// ------------------------

void Input::PollMouse()
{
    double x, y;
    m_backend->GetMousePosition(x, y);

    if (m_firstMouse)
    {
        m_lastMouseX = x;
        m_lastMouseY = y;
        m_firstMouse = false;
    }

    m_mouseDX = float(x - m_lastMouseX);
    m_mouseDY = float(m_lastMouseY - y); // FPS style

    m_lastMouseX = x;
    m_lastMouseY = y;
}

// ------------------------
// Context resolution
// ------------------------

void Input::ResolveContexts()
{
    std::unordered_map<int, bool> consumed;

    for (auto it = m_contexts.rbegin(); it != m_contexts.rend(); ++it)
    {
        for (const Binding& b : (*it)->m_bindings)
        {
            if (consumed[b.key])
                continue;

            KeyState ks = m_keys[b.key];

            bool isActive = ks == KeyState::Pressed || ks == KeyState::Down;
            bool isReleased = ks == KeyState::Released;

            if (b.type == BindingType::Action)
            {
                if (ks == KeyState::Pressed)
                    m_actionPressed[b.name] = true;
                if (ks == KeyState::Released)
                    m_actionReleased[b.name] = true;
                if (isActive)
                    m_actionDown[b.name] = true;
            }
            else if (b.type == BindingType::Axis)
            {
                if (isActive)
                    m_axes[b.name] += b.scale;
            }

            if (b.consume && (isActive || isReleased))
                consumed[b.key] = true;
        }
    }

    for (auto& [name, value] : m_axes)
        value = std::clamp(value, -1.f, 1.f);
}

// ------------------------
// Context API
// ------------------------

std::shared_ptr<Input::Context> Input::CreateContext()
{
    return std::make_shared<Context>();
}

void Input::Context::BindAction(int key, const std::string& name, bool consume)
{
    m_bindings.push_back({ key, name, BindingType::Action, 1.f, consume });
}

void Input::Context::BindAxis(int key, const std::string& name, float scale)
{
    m_bindings.push_back({ key, name, BindingType::Axis, scale, false });
}

void Input::PushContext(const std::shared_ptr<Context>& ctx)
{
    m_contexts.push_back(ctx);
}

void Input::PopContext()
{
    if (!m_contexts.empty())
        m_contexts.pop_back();
}

// ------------------------
// Gameplay API
// ------------------------

bool Input::Action(const std::string& name) const
{
    auto it = m_actionDown.find(name);
    return it != m_actionDown.end() && it->second;
}

bool Input::ActionPressed(const std::string& name) const
{
    auto it = m_actionPressed.find(name);
    return it != m_actionPressed.end() && it->second;
}

bool Input::ActionReleased(const std::string& name) const
{
    auto it = m_actionReleased.find(name);
    return it != m_actionReleased.end() && it->second;
}

float Input::Axis(const std::string& name) const
{
    auto it = m_axes.find(name);
    return it != m_axes.end() ? it->second : 0.f;
}
