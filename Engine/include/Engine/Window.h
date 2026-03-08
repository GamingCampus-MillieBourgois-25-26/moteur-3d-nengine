#pragma once
/**
 * @file Window.h
 * @brief GLFW window wrapper for the engine.
 * @ingroup Engine
 */

#include <iostream>

struct GLFWwindow; ///< Forward declaration of the GLFW window handle.

namespace Engine
{
    /**
     * @brief Wraps a GLFW window and exposes a minimal lifecycle API.
     *
     * WindowInstance is owned by Application and created before the
     * Direct3D device is initialised.  The GLFW window handle is passed
     * to the Renderer and the Input system.
     */
    class WindowInstance
    {
    public:
        /**
         * @brief Creates a GLFW window with the given dimensions and title.
         * @param width   Window width in pixels.
         * @param height  Window height in pixels.
         * @param title   UTF-8 window title string.
         */
        void Create(int width, int height, const char* title);

        /**
         * @brief Polls GLFW events (should be called once per frame).
         */
        void Update();

        /**
         * @brief Returns true if the user has requested to close the window.
         */
        bool ShouldClose() const;

        /**
         * @brief Returns the underlying GLFWwindow handle.
         * @return Raw GLFW window pointer (non-owning).
         */
        GLFWwindow* GetGLFWwindow() const { return m_window; }

    private:
        GLFWwindow* m_window = nullptr; ///< Underlying GLFW window handle.
    };
} // namespace Engine
