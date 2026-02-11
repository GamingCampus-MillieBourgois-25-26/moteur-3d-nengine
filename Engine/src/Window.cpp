#include "Engine/Window.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Engine {
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
	}

	void WindowInstance::Update()
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	bool WindowInstance::ShouldClose() const
	{
		return glfwWindowShouldClose(m_window);
	}

}