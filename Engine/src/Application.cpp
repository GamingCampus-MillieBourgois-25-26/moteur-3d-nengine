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
	//loader.loadOBJFile();

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
	coord.RegisterComponent<MaterialData>();

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
	renderSignature.set(coord.GetComponentType<MaterialData>(), true);

	coord.SetSystemSignature<RenderSystem>(renderSignature);

	// 4. Create an entity
	Entity e = coord.CreateEntity();

	// 5. Add Transform
	Transform tr;
	tr.position = { 0, 0, 0 };
	tr.scale = { 0.5, 0.5, 0.5 };
	tr.rotation = { 0, 0, 180, 1 }; // quaternion
	coord.AddComponent(e, tr);

	// Chargement du modele .obj
	OBJResult obj = LoadOBJ("OBJ/SpinCat.obj");

	// Création des buffers GPU
	MeshRenderer mr = renderer.CreateMeshRenderer(obj.mesh);

	// Ajout du MeshRender à l'entité pour le RenderSystem
	coord.AddComponent(e, mr);
	
	// Charger la texture diffuse automatiquement
	MaterialData mat;
	if (!mat.diffuse) {
		std::cout << "ERREUR: texture diffuse NULL" << std::endl;
	}

	mat.diffuse = renderer.CreateTextureFromFile(
		L"OBJ/" + std::wstring(obj.material.diffuseTexName.begin(), obj.material.diffuseTexName.end())
	);

	// Ajouter le composant Material
	coord.AddComponent(e, mat);
	
	// 10. Add Velocity
	Velocity vel;
	vel.velocity = { 0, 0, 0 };
	coord.AddComponent(e, vel);

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

		audio.Update();
		window.Update();
		movementSystem->Update(coord, dt);
		renderSystem->Render(coord, renderer);

		input->Update();

		speed = 2.0f * dt;


		renderer.MoveCamera(
			input->Axis("MoveRight") * speed,
			input->Axis("MoveUp") * speed,
			input->Axis("MoveForward") * speed
		);
		//std::cout << input->Action("LockCamera") << std::endl;
		if (input->Action("LockCamera")) {
			glfwSetInputMode(window.GetGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			renderer.RotateCamera(
				input->MouseDX() * mouseSensitivity,
				input->MouseDY() * mouseSensitivity
			);
		}
		else glfwSetInputMode(window.GetGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		
	}
	Shutdown();
}

void Engine::Application::Shutdown()
{
    std::cout << "Shutting down application...\n";

    window.ShouldClose();
    isRunning = false;
}
