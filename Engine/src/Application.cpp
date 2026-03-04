#include "Engine/Application.h"
#include <GLFW/glfw3.h> 
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <chrono>

using namespace std::chrono;
namespace fs = std::filesystem;

void Engine::Application::Init()
{
	std::cout << "Initializing application...\n";

	window.Create(800, 600, "My Application");

	input = CreateGLFWInput(window.GetGLFWwindow());
	// Créer l'input avec le backend GLFW

	// Créer le contexte caméra
	auto camCtx = input->CreateContext();
	camCtx->BindAxis(GLFW_KEY_W, "MoveForward", 1.f);
	camCtx->BindAxis(GLFW_KEY_S, "MoveForward", -1.f);
	camCtx->BindAxis(GLFW_KEY_D, "MoveRight", -1.f);
	camCtx->BindAxis(GLFW_KEY_A, "MoveRight", 1.f);
	camCtx->BindAxis(GLFW_KEY_E, "MoveUp", 1.f);
	camCtx->BindAxis(GLFW_KEY_Q, "MoveUp", -1.f);
	camCtx->BindAction(GLFW_KEY_SPACE, "LockCamera");
	input->PushContext(camCtx);

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

	// 1. Register components (on enregistre les composants qu'on va donner)
	coord.RegisterComponent<Transform>();
	coord.RegisterComponent<Velocity>();
	coord.RegisterComponent<MeshRenderer>();

	// Enregistrer le MovementSystem
	movementSystem = coord.RegisterSystem<MovementSystem>();

	Signature movementSignature;
	movementSignature.set(coord.GetComponentType<Transform>(), true);
	movementSignature.set(coord.GetComponentType<Velocity>(), true);

	coord.SetSystemSignature<MovementSystem>(movementSignature);

	// 2. Register RenderSystem
	renderSystem = coord.RegisterSystem<RenderSystem>();

	// 3. Define signature for RenderSystem
	Signature renderSignature;
	renderSignature.set(coord.GetComponentType<Transform>(), true);
	renderSignature.set(coord.GetComponentType<MeshRenderer>(), true);

	coord.SetSystemSignature<RenderSystem>(renderSignature);

	// Register PhysicsBodySystem
	physicsBodySystem = coord.RegisterSystem<PhysicsBodySystem>();

	Signature physicsSignature;
	physicsSignature.set(coord.GetComponentType<Transform>(), true);

	coord.SetSystemSignature<PhysicsBodySystem>(physicsSignature);

	physicsBodySystem->Init();

	// 4. Create an entity
	Entity e = coord.CreateEntity();

	// 5. Add Transform
	Transform tr;
	tr.position = { 0, 2, 0 };
	tr.scale = { 0.5, 0.5, 0.5 };
	tr.rotation = { 0, 0, 0, 1 }; // quaternion
	coord.AddComponent(e, tr);

	// 6. Add MeshRenderer
	MeshRenderer mr;
	mr.vertexBuffer = renderer.GetMesh().vertexBuffer;   // on va ajouter GetMesh()
	mr.indexBuffer = renderer.GetMesh().indexBuffer;
	mr.indexCount = renderer.GetMesh().indexCount;
	coord.AddComponent(e, mr);

	// 7. Add Velocity
	Velocity vel;
	vel.velocity = { 0, 0, 0 };
	coord.AddComponent(e, vel);

	// 8. Add a dynamic rigid body (mass=1, box half-extents match scale)
	physicsBodySystem->AddRigidBody(e, coord, 1.0f, btVector3(0.5f, 0.5f, 0.5f));

	// 9. (Optional) Create a static ground plane
	Entity ground = coord.CreateEntity();

	Transform groundTr;
	groundTr.position = { 0, -1, 0 };
	groundTr.scale = { 10, 0.1f, 10 };
	groundTr.rotation = { 0, 0, 0, 1 };
	coord.AddComponent(ground, groundTr);

	// mass = 0 → static object (won't move, acts as ground)
	physicsBodySystem->AddRigidBody(ground, coord, 0.0f, btVector3(10.0f, 0.1f, 10.0f));

	isRunning = true;

	Running();
}

void Engine::Application::Running()
{
	auto lastTime = clock::now();
	std::cout << "Application is running...\n";

	while (isRunning && !window.ShouldClose())
	{
		auto now = clock::now();
		std::chrono::duration<float> elapsed = now - lastTime;
		float dt = elapsed.count();
		lastTime = now;

		speed = 2.0f * dt;
		audio.Update();
		window.Update();

		// Step physics BEFORE movement/render so transforms are up-to-date
		physicsBodySystem->Update(coord, dt);

		movementSystem->Update(coord, dt);
		renderSystem->Render(coord, renderer);

		input->Update();


		//std::cout << input->Action("LockCamera") << std::endl;
		if (input->Action("LockCamera")) {
			glfwSetInputMode(window.GetGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			renderer.RotateCamera(
				input->MouseDX() * mouseSensitivity,
				input->MouseDY() * mouseSensitivity
			);
		}
		else glfwSetInputMode(window.GetGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		renderer.MoveCamera(
			input->Axis("MoveRight") * speed,
			input->Axis("MoveUp") * speed,
			input->Axis("MoveForward") * speed
		);
	}

	Shutdown();
}

void Engine::Application::Shutdown()
{
	std::cout << "Shutting down application...\n";

    physicsBodySystem->Shutdown();

    window.ShouldClose();
    isRunning = false;
}
