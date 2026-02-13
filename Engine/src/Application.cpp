#include "Engine/Application.h"
// L'equipe je vais ecrire en anglais c'est plus facile ;;


void Engine::Application::Init()
{
	std::cout << "Initializing application...\n";

	window.Create(800, 600, "My Application");

	if (!renderer.Initialize(window.GetGLFWwindow(), 800, 600))
	{
		std::cout << "Failed to initialize renderer\n";
		return;
	}

	isRunning = true;
}


void Engine::Application::Running()
{
	auto lastTime = clock::now();
	std::cout << "Application is running...\n";
	while (isRunning && !window.ShouldClose()) {
		// Update application state, handle input, render, etc.

		auto now = clock::now();
		std::chrono::duration<float> elapsed = now - lastTime;
		float dt = elapsed.count();
		lastTime = now;

		window.Update();
		renderer.Render(dt);

	}

}

void Engine::Application::Shutdown()
{
	std::cout << "Shutting down application...\n";
	window.ShouldClose();
	isRunning = false;
}

void Engine::Application::Run()
{
	std::cout << "TA GUEULE\n";
}