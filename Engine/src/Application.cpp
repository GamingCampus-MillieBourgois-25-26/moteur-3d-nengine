#include "Engine/Application.h"
// L'equipe je vais ecrire en anglais c'est plus facile ;;
void Engine::Application::Init()
{
	std::cout << "Initializing application...\n";
	// open 

	window.Create(800, 600, "My Application");

	if (!renderer.Initialize(window.GetGLFWwindow()))
	{
		std::cout << "Failed to initialize renderer\n";
		return;
	}
	else
		renderer.Initialize(window.GetGLFWwindow()) == true;


	isRunning = true;
}

void Engine::Application::Running()
{
	std::cout << "Application is running...\n";
	while (isRunning && !window.ShouldClose()) {
		// Update application state, handle input, render, etc.
		std::cout << "Running...\n";
		
		window.Update();
		renderer.Render();

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