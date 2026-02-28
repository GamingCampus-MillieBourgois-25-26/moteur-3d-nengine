#include "Engine/Application.h"
// L'equipe je vais ecrire en anglais c'est plus facile ;;

void Engine::Application::Init()
{
	std::cout << "Initializing application...\n";

	window.Create(800, 600, "My Application");


  
	audio.Init();
	audio.LoadBanks();
	audio.PlayEvent("event:/MSC_EFN");
	loader.loadOBJFile();

  

	if (!renderer.Initialize(window.GetGLFWwindow(), 800, 600))

	{
		std::cout << "Failed to initialize renderer\n";
		return;
	}

	// ECS

	coord.Init();

	// 1. Register components
	coord.RegisterComponent<Transform>();
	coord.RegisterComponent<MeshRenderer>();

	// 2. Register RenderSystem
	renderSystem = coord.RegisterSystem<RenderSystem>();

	// 3. Define signature for RenderSystem
	Signature signature;
	signature.set(coord.GetComponentType<Transform>(), true);
	signature.set(coord.GetComponentType<MeshRenderer>(), true);
	coord.SetSystemSignature<RenderSystem>(signature);

	// 4. Create an entity
	Entity e = coord.CreateEntity();

	// 5. Add Transform
	Transform tr;
	tr.position = { 0, 0, 0 };
	tr.scale = { 1, 1, 1 };
	tr.rotation = { 0, 0, 0, 1 }; // quaternion
	coord.AddComponent(e, tr);

	// 6. Add MeshRenderer
	MeshRenderer mr;
	mr.vertexBuffer = renderer.GetMesh().vertexBuffer;   // on va ajouter GetMesh()
	mr.indexBuffer = renderer.GetMesh().indexBuffer;
	mr.indexCount = renderer.GetMesh().indexCount;
	coord.AddComponent(e, mr);

	/*for (int i = 0; i < 10; i++)
	{
		Entity e = coord.CreateEntity();

		Transform tr;
		tr.position = { float(i), 0, 0 };
		tr.scale = { 1, 1, 1 };
		tr.rotation = { 0, 0, 0, 1 };
		coord.AddComponent(e, tr);
		coord.AddComponent(e, mr);
	}*/

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

		audio.Update();
		window.Update();
		//renderer.Render(dt);
		renderSystem->Render(coord, renderer);
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