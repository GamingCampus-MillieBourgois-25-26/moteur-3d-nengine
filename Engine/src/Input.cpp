#include "Engine/Input.h"
#include <GLFW/glfw3.h>

// ========================
// GLFW Backend
// ========================

class GLFWBackend : public IInputBackend
{
public:
    explicit GLFWBackend(GLFWwindow* window)
        : m_window(window)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetWindowUserPointer(m_window, this);

        glfwSetScrollCallback(m_window,
            [](GLFWwindow* window, double xoffset, double yoffset)
            {
                auto backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(window));
                backend->m_scrollX += static_cast<float>(xoffset);
                backend->m_scrollY += static_cast<float>(yoffset);
            });
    }

    void PollEvents() override { glfwPollEvents(); }

    bool IsKeyDown(int key) const override
    {
        return glfwGetKey(m_window, key) == GLFW_PRESS;
    }

    bool IsMouseButtonDown(int button) const override
    {
        return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
    }

    void GetMousePosition(double& x, double& y) const override
    {
        glfwGetCursorPos(m_window, &x, &y);
    }

    float ConsumeScrollX() override
    {
        float tmp = m_scrollX;
        m_scrollX = 0.f;
        return tmp;
    }

    float ConsumeScrollY() override
    {
        float tmp = m_scrollY;
        m_scrollY = 0.f;
        return tmp;
    }

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
// Keyboard
// ========================

void Input::PollKeyboard()
{
    for (int key = 0; key < KEY_COUNT; ++key)
    {
        auto& ks = m_keys[key];
        bool pressed = m_backend->IsKeyDown(key);

        if (pressed)
            ks = (ks == KeyState::Up || ks == KeyState::Released)
            ? KeyState::Pressed
            : KeyState::Down;
        else
            ks = (ks == KeyState::Down || ks == KeyState::Pressed)
            ? KeyState::Released
            : KeyState::Up;
    }
}

// ========================
// Mouse Buttons
// ========================

void Input::PollMouseButtons()
{
    for (int button = 0; button < MOUSE_BUTTON_COUNT; ++button)
    {
        auto& ks = m_mouseButtons[button];
        bool pressed = m_backend->IsMouseButtonDown(button);

        if (pressed)
            ks = (ks == KeyState::Up || ks == KeyState::Released)
            ? KeyState::Pressed
            : KeyState::Down;
        else
            ks = (ks == KeyState::Down || ks == KeyState::Pressed)
            ? KeyState::Released
            : KeyState::Up;
    }
}

// ========================
// Mouse Movement
// ========================

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

    m_mouseDX = static_cast<float>(x - m_lastMouseX);
    m_mouseDY = static_cast<float>(m_lastMouseY - y);

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
        for (const Binding& b : (*it)->m_bindings)
        {
            if (consumed[b.key])
                continue;

            KeyState ks;

            if (b.key >= MOUSE_OFFSET)
                ks = m_mouseButtons[b.key - MOUSE_OFFSET];
            else
                ks = m_keys[b.key];

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

void Input::PushContext(const std::shared_ptr<Context>& ctx)
{
    m_contexts.push_back(ctx);
}

void Input::PopContext()
{
    if (!m_contexts.empty())
        m_contexts.pop_back();
}

// ========================
// Gameplay API
// ========================

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
