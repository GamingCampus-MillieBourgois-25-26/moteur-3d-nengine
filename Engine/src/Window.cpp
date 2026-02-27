#include "Engine/Window.h"
#include "Engine/Renderer.h"   // ← IMPORTANT
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#define GLFW_EXPOSE_NATIVE_WIN32

// Renderer global pour les callbacks
static Renderer* g_renderer = nullptr;

namespace Engine {

    void WindowInstance::SetRenderer(Renderer* renderer)
    {
        g_renderer = renderer;
    }

    // --- CALLBACKS GLFW ---

    static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (g_renderer)
            g_renderer->OnMouseMove(xpos, ypos);
    }

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (g_renderer)
            g_renderer->OnMouseButton(button, action);
    }

    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        if (g_renderer)
            g_renderer->OnScroll(yoffset);
    }

    // --- CREATION DE LA FENÊTRE ---

    void WindowInstance::Create(int width, int height, const char* title)
    {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            return;
        }

        m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_window) {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_window);

        // Enregistrer les callbacks
        glfwSetCursorPosCallback(m_window, MouseMoveCallback);
        glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
        glfwSetScrollCallback(m_window, ScrollCallback);

        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void WindowInstance::Update()
    {
        glfwPollEvents();
    }

    bool WindowInstance::ShouldClose() const
    {
        return glfwWindowShouldClose(m_window);
    }

}
