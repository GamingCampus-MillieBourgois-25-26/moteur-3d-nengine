#include "Engine/Application.h"

// L'equipe je vais ecrire en anglais c'est plsu facile ;;
void Engine::Application::Init()
{
	std::cout << "Initializing application...\n";
	// open 
	isRunning = true;
	window.Create(800, 600, "My Application");
	audio.Init();
	audio.LoadBanks();
	audio.PlayEvent("event:/MSC_DS3");

	Running();
}

void Engine::Application::Running()
{
	std::cout << "Application is running...\n";

	while (isRunning) {
		// Update application state, handle input, render, etc.
		//std::cout << "Running...\n";
		audio.Update();
		window.Update();
	}

	Shutdown();
}

void Engine::Application::Shutdown()
{
	audio.Shutdown();
	std::cout << "Shutting down application...\n";
	window.ShouldClose();
	isRunning = false;
}

void Engine::Application::Run()
{
	std::cout << "TA GUEULE\n";
}