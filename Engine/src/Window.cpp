#include "Engine/Window.h"

namespace Engine {
	void Window::Create(int width, int height, const char* title)
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
	}

	void Window::Update()
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_window);
	}

}