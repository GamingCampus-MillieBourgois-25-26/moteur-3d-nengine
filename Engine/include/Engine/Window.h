#pragma once
#include <iostream>

struct GLFWwindow;

namespace Engine {
	class WindowInstance
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