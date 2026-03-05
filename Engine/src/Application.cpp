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

	// Créer l'input avec le backend GLFW
	input = CreateGLFWInput(window.GetGLFWwindow());

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
	audio.PlayEvent("event:/MSC_OIIA");

	if (!renderer.Initialize(window.GetGLFWwindow(), 800, 600))
	{
		std::cout << "Failed to initialize renderer\n";
		return;
	}

	// ECS

	coord.Init();

	// 1. Register components
	coord.RegisterComponent<Transform>();
	coord.RegisterComponent<Velocity>();
	coord.RegisterComponent<MeshRenderer>();
	coord.RegisterComponent<MaterialData>();

	coord.RegisterComponent<Collider>();
	coord.RegisterComponent<Trigger>();
	coord.RegisterComponent<Force>();
	coord.RegisterComponent<Joint>();
	coord.RegisterComponent<CharacterController>();

	// 2. Register PhysicsBodySystem (owns the Bullet world + simulation)
	physicsBodySystem = coord.RegisterSystem<PhysicsBodySystem>();

	Signature physicsSignature;
	physicsSignature.set(coord.GetComponentType<Transform>(), true);
	coord.SetSystemSignature<PhysicsBodySystem>(physicsSignature);

	physicsBodySystem->Init();

	// 3. Register ColliderSystem (creates shapes → delegates to PhysicsBodySystem)
	colliderSystem = coord.RegisterSystem<ColliderSystem>();

	Signature colliderSignature;
	colliderSignature.set(coord.GetComponentType<Transform>(), true);
	colliderSignature.set(coord.GetComponentType<Collider>(), true);
	coord.SetSystemSignature<ColliderSystem>(colliderSignature);

	colliderSystem->Init(physicsBodySystem);

	// 4. Register ForceSystem (applies forces/impulses → delegates to PhysicsBodySystem)
	forceSystem = coord.RegisterSystem<ForceSystem>();

	Signature forceSignature;
	forceSignature.set(coord.GetComponentType<Force>(), true);
	coord.SetSystemSignature<ForceSystem>(forceSignature);

	forceSystem->Init(physicsBodySystem);

	// 5. Register JointSystem (creates constraints between entities)
	jointSystem = coord.RegisterSystem<JointSystem>();

	Signature jointSignature;
	jointSignature.set(coord.GetComponentType<Joint>(), true);
	coord.SetSystemSignature<JointSystem>(jointSignature);


	jointSystem->Init(physicsBodySystem);

	// 6. Register CharacterControllerSystem (manages playable characters)
	characterControllerSystem = coord.RegisterSystem<CharacterControllerSystem>();

	Signature ccSignature;
	ccSignature.set(coord.GetComponentType<Transform>(), true);
	ccSignature.set(coord.GetComponentType<CharacterController>(), true);
	coord.SetSystemSignature<CharacterControllerSystem>(ccSignature);

	characterControllerSystem->Init(physicsBodySystem);

	// 7. Register MovementSystem
	movementSystem = coord.RegisterSystem<MovementSystem>();

	Signature movementSignature;
	movementSignature.set(coord.GetComponentType<Transform>(), true);
	movementSignature.set(coord.GetComponentType<Velocity>(), true);
	coord.SetSystemSignature<MovementSystem>(movementSignature);

	// 8. Register RenderSystem
	renderSystem = coord.RegisterSystem<RenderSystem>();

	Signature renderSignature;
	renderSignature.set(coord.GetComponentType<Transform>(), true);
	renderSignature.set(coord.GetComponentType<MeshRenderer>(), true);
	renderSignature.set(coord.GetComponentType<MaterialData>(), true);

	coord.SetSystemSignature<RenderSystem>(renderSignature);

	// 9. Register TriggerSystem (for managing trigger events)
	triggerSystem = coord.RegisterSystem<TriggerSystem>();

	Signature triggerSignature;
	triggerSignature.set(coord.GetComponentType<Transform>(), true);
	triggerSignature.set(coord.GetComponentType<Collider>(), true);
	triggerSignature.set(coord.GetComponentType<Trigger>(), true);
	coord.SetSystemSignature<TriggerSystem>(triggerSignature);

	triggerSystem->Init(physicsBodySystem);

	// 10. Create a dynamic entity
	Entity e = coord.CreateEntity();

	Transform tr;
	tr.position = { 0, 2, 0 };
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

	mat.diffuse = renderer.CreateTextureFromFile(
		L"OBJ/" + std::wstring(obj.material.diffuseTexName.begin(), obj.material.diffuseTexName.end())
	);

	// Collider → ColliderSystem creates the shape, PhysicsBodySystem gets the rigid body
	Collider col;
	col.shapeType   = ColliderShapeType::Box;
	col.halfExtents = { 0.5f, 0.5f, 0.5f };
	col.mass        = 1.0f;
	coord.AddComponent(e, col);

	// Force → ForceSystem applies forces/impulses on the rigid body
	Force force;
	force.force  = { 0.0f, 0.0f, 0.0f }; // No continuous linear force
	force.torque = { 0.0f, 2.5f, 0.0f }; // Gentle continuous torque around Y axis
	force.mode   = ForceMode::Force;      // Continuous force (not impulse)
	force.active = true;
	coord.AddComponent(e, force);

	// 11. Create a trigger entity
	Entity triggerEntity = coord.CreateEntity();

	Transform triggerTr;
	triggerTr.position = { 1, 0, 1 };
	triggerTr.scale = { 1, 1, 1 };
	triggerTr.rotation = { 0, 0, 0, 1 };
	coord.AddComponent(triggerEntity, triggerTr);

	Collider triggerCol;
	triggerCol.shapeType   = ColliderShapeType::Box;
	triggerCol.halfExtents = { 1.0f, 1.0f, 1.0f };
	triggerCol.mass        = 0.0f; // static
	coord.AddComponent(triggerEntity, triggerCol);

	Trigger trigger;
	coord.AddComponent(triggerEntity, trigger);

	// Create a zone entity
	Entity zone = coord.CreateEntity();

	Transform zoneTransform;
	zoneTransform.position = { 0, 1, 0 };
	zoneTransform.scale = { 2, 2, 2 };
	zoneTransform.rotation = { 0, 0, 0, 1 };
	coord.AddComponent(zone, zoneTransform);

	Collider zoneCollider;
	zoneCollider.shapeType   = ColliderShapeType::Box;
	zoneCollider.halfExtents = { 2.0f, 2.0f, 2.0f };
	zoneCollider.mass        = 0.0f;
	coord.AddComponent(zone, zoneCollider);

	Trigger trig;
	trig.onEnter = [](Entity self, Entity other) {
	    std::cout << "Entity " << other << " entered zone " << self << "\n";
	};
	trig.onExit = [](Entity self, Entity other) {
	    std::cout << "Entity " << other << " left zone " << self << "\n";
	};
	coord.AddComponent(zone, trig);

	// Ground plane (static)
	Entity ground = coord.CreateEntity();

	Transform groundTr;
	groundTr.position = { 0, -1, 0 };
	groundTr.scale = { 1, 1, 1 };
	groundTr.rotation = { 0, 0, 0, 1 };
	coord.AddComponent(ground, groundTr);

	Collider groundCol;
	groundCol.shapeType   = ColliderShapeType::Box;
	groundCol.halfExtents = { 50.0f, 1.0f, 50.0f };
	groundCol.mass        = 0.0f; // static
	coord.AddComponent(ground, groundCol);


	if (!mat.diffuse) std::cout << "Erreur : texture introuvable." << std::endl;

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

		speed = 2.0f * dt;
		audio.Update();
		window.Update();

		// 1. ColliderSystem: detect new Collider components → create shapes
		colliderSystem->Update(coord);

		// 2. JointSystem: detect new Joint components → create constraints
		jointSystem->Update(coord);

		// 3. ForceSystem: apply pending forces/impulses before stepping the simulation
		forceSystem->Update(coord);

		// 4. CharacterControllerSystem: create controllers + apply walk/jump
		characterControllerSystem->Update(coord, dt);

		// 5. PhysicsBodySystem: step simulation + sync transforms
		physicsBodySystem->Update(coord, dt);
		triggerSystem->Update(coord);
		movementSystem->Update(coord, dt);
		renderSystem->Render(coord, renderer);

		input->Update();


		speed = 2.0f * dt;


		renderer.MoveCamera(
			input->Axis("MoveRight") * speed,
			input->Axis("MoveUp") * speed,
			input->Axis("MoveForward") * speed
		);

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

    characterControllerSystem->Shutdown();
    jointSystem->Shutdown();
    physicsBodySystem->Shutdown();

    window.ShouldClose();
    isRunning = false;
}
