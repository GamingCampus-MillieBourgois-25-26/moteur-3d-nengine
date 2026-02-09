#include "Engine/Input.h"
#include <GLFW/glfw3.h>

Input::Input(GLFWwindow* window)
    : m_window(window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::Update()
{
    glfwPollEvents();

    PollKeyboard();
    PollMouse();

    // Reset actions & axes
    m_axes.clear();
    m_actions.clear();

    // Resolve contexts (top = priority)
    for (auto it = m_contexts.rbegin(); it != m_contexts.rend(); ++it)
    {
        for (const auto& b : (*it)->bindings)
        {
            auto keyIt = m_keys.find(b.key);
            if (keyIt == m_keys.end())
                continue;

            if (keyIt->second == KeyState::Pressed ||
                keyIt->second == KeyState::Down)
            {
                m_axes[b.action] += b.scale;
                m_actions[b.action] = true;
            }
        }
    }
}

void Input::PollKeyboard()
{
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
    {
        int state = glfwGetKey(m_window, key);
        auto& ks = m_keys[key];

        if (state == GLFW_PRESS)
        {
            if (ks == KeyState::Up || ks == KeyState::Released)
                ks = KeyState::Pressed;
            else
                ks = KeyState::Down;
        }
        else
        {
            if (ks == KeyState::Down || ks == KeyState::Pressed)
                ks = KeyState::Released;
            else
                ks = KeyState::Up;
        }
    }
}

void Input::PollMouse()
{
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);

    if (m_firstMouse)
    {
        m_lastMouseX = x;
        m_lastMouseY = y;
        m_firstMouse = false;
    }

    m_mouseDX = float(x - m_lastMouseX);
    m_mouseDY = float(y - m_lastMouseY);

    m_lastMouseX = x;
    m_lastMouseY = y;
}

void Input::PushContext(Context* ctx)
{
    m_contexts.push_back(ctx);
}

void Input::PopContext()
{
    if (!m_contexts.empty())
        m_contexts.pop_back();
}

bool Input::Action(const std::string& name) const
{
    auto it = m_actions.find(name);
    return it != m_actions.end() && it->second;
}

float Input::Axis(const std::string& name) const
{
    auto it = m_axes.find(name);
    return it != m_axes.end() ? it->second : 0.f;
}

float Input::MouseDX() const { return m_mouseDX; }
float Input::MouseDY() const { return m_mouseDY; }
