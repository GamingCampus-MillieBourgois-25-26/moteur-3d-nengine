#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine {
	class Window
	{
	public:
		void Create(int width, int height, const char* title);
		void Update();
		bool ShouldClose() const;
		GLFWwindow* GetGLFWwindow() const { return m_window; }
	private:
		GLFWwindow* m_window = nullptr;
	};
}