#include "Engine/Input.h"
#include <GLFW/glfw3.h>
//#include <Xinput.h>
#include <Engine/json.hpp>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

// ========================
// GLFW Backend
// ========================

class GLFWBackend : public IInputBackend
{
public:
    explicit GLFWBackend(GLFWwindow* window) : m_window(window)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetWindowUserPointer(m_window, this);

        glfwSetScrollCallback(m_window,
            [](GLFWwindow* window, double x, double y)
            {
                auto b = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window));
                b->m_scrollX += (float)x;
                b->m_scrollY += (float)y;
            });
    }

    void PollEvents() override { glfwPollEvents(); }
    bool IsKeyDown(int key) const override { return glfwGetKey(m_window, key) == GLFW_PRESS; }
    bool IsMouseButtonDown(int button) const override { return glfwGetMouseButton(m_window, button) == GLFW_PRESS; }
    void GetMousePosition(double& x, double& y) const override { glfwGetCursorPos(m_window, &x, &y); }

    float ConsumeScrollX() override { float t = m_scrollX; m_scrollX = 0.f; return t; }
    float ConsumeScrollY() override { float t = m_scrollY; m_scrollY = 0.f; return t; }

private:
    GLFWwindow* m_window;
    float m_scrollX = 0.f;
    float m_scrollY = 0.f;
};

// ========================
// Input Implementation
// ========================

Input::Input(std::unique_ptr<IInputBackend> backend)
    : m_backend(std::move(backend))
{
    m_keys.fill(KeyState::Up);
    m_mouseButtons.fill(KeyState::Up);
}

void Input::Update()
{
    m_backend->PollEvents();
    PollKeyboard();
    PollMouseButtons();
    PollMouse();

    m_scrollX = m_backend->ConsumeScrollX();
    m_scrollY = m_backend->ConsumeScrollY();

    m_actionDown.clear();
    m_actionPressed.clear();
    m_actionReleased.clear();
    m_axes.clear();

    ResolveContexts();
}

// ========================
// Polling
// ========================

void Input::PollKeyboard()
{
    for (int i = 0; i < KEY_COUNT; ++i)
    {
        bool down = m_backend->IsKeyDown(i);
        auto& k = m_keys[i];

        if (down)
            k = (k == KeyState::Up || k == KeyState::Released) ? KeyState::Pressed : KeyState::Down;
        else
            k = (k == KeyState::Down || k == KeyState::Pressed) ? KeyState::Released : KeyState::Up;
    }
}

void Input::PollMouseButtons()
{
    for (int i = 0; i < MOUSE_BUTTON_COUNT; ++i)
    {
        bool down = m_backend->IsMouseButtonDown(i);
        auto& k = m_mouseButtons[i];

        if (down)
            k = (k == KeyState::Up || k == KeyState::Released) ? KeyState::Pressed : KeyState::Down;
        else
            k = (k == KeyState::Down || k == KeyState::Pressed) ? KeyState::Released : KeyState::Up;
    }
}

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

    m_mouseDX = (float)(x - m_lastMouseX);
    m_mouseDY = (float)(m_lastMouseY - y);

    m_lastMouseX = x;
    m_lastMouseY = y;
}

// ========================
// Context Resolution
// ========================

void Input::ResolveContexts()
{
    std::unordered_map<int, bool> consumed;

    for (auto it = m_contexts.rbegin(); it != m_contexts.rend(); ++it)
    {
        for (const auto& b : (*it)->m_bindings)
        {
            if (consumed[b.key]) continue;

            KeyState ks = KeyState::Up;

            if (b.key >= MOUSE_OFFSET)
                ks = m_mouseButtons[b.key - MOUSE_OFFSET];
            else
                ks = m_keys[b.key];

            bool active = ks == KeyState::Pressed || ks == KeyState::Down;

            if (b.type == BindingType::Action)
            {
                if (ks == KeyState::Pressed) m_actionPressed[b.name] = true;
                if (ks == KeyState::Released) m_actionReleased[b.name] = true;
                if (active) m_actionDown[b.name] = true;
            }
            else if (active)
                m_axes[b.name] += b.scale;

            if (b.consume && (active || ks == KeyState::Released))
                consumed[b.key] = true;
        }
    }

    for (auto& [_, v] : m_axes)
        v = std::clamp(v, -1.f, 1.f);
}

// ========================
// Context API
// ========================

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

void Input::Context::Rebind(const std::string& name, int newKey)
{
    for (auto& b : m_bindings)
        if (b.name == name)
            b.key = newKey;
}

std::vector<Input::Context::SerializableBinding> Input::Context::Serialize() const
{
    std::vector<SerializableBinding> out;
    for (const auto& b : m_bindings)
        out.push_back({ b.name, b.key, b.type, b.scale, b.consume });
    return out;
}

void Input::Context::Deserialize(const std::vector<SerializableBinding>& data)
{
    m_bindings.clear();
    for (const auto& b : data)
        m_bindings.push_back({ b.key, b.name, b.type, b.scale, b.consume });
}

// ========================
// Gameplay API
// ========================

bool Input::Action(const std::string& name) const
{
    return m_actionDown.contains(name);
}

bool Input::ActionPressed(const std::string& name) const
{
    return m_actionPressed.contains(name);
}

bool Input::ActionReleased(const std::string& name) const
{
    return m_actionReleased.contains(name);
}

float Input::Axis(const std::string& name) const
{
    auto it = m_axes.find(name);
    return it != m_axes.end() ? it->second : 0.f;
}

int Input::GetAnyPressedKey() const
{
    for (int i = 0; i < KEY_COUNT; ++i)
        if (m_keys[i] == KeyState::Pressed)
            return i;

    for (int i = 0; i < MOUSE_BUTTON_COUNT; ++i)
        if (m_mouseButtons[i] == KeyState::Pressed)
            return MOUSE_OFFSET + i;

    return -1;
}
