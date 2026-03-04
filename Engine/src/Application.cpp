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
    coord.RegisterComponent<Name>();
    coord.RegisterComponent<Script>();


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

    // 4. Create an entity
    Entity e = coord.CreateEntity();

    // 5. Add Transform
    Transform tr;
    tr.position = { 0, 0, 0 };
    tr.scale = { 0.5f, 0.5f, 0.5f };
    tr.rotation = { 0, 0, 0, 1 }; // quaternion
    coord.AddComponent<::Transform>(e, tr);

    // 6. Add MeshRenderer
    MeshRenderer mr;
    mr.vertexBuffer = renderer.GetMesh().vertexBuffer;   // on va ajouter GetMesh()
    mr.indexBuffer = renderer.GetMesh().indexBuffer;
    mr.indexCount = renderer.GetMesh().indexCount;
    coord.AddComponent<MeshRenderer>(e, mr);

    // Add the initial entity to the scene list for UI
    m_sceneEntities.push_back(e);

    // 7. Add Velocity
    Velocity vel;
    vel.velocity = { 0, 0, 0 };
    coord.AddComponent<Velocity>(e, vel);

    // 8. Add Name
	Name name;
	name.name = "Entity";
	coord.AddComponent<Name>(e, name);

    // 9. Add Script
    Script script;
    script.className = "Script";
	coord.AddComponent<Script>(e, script);

    isRunning = true;
}

void Engine::Application::Update(float dt)
{
    // une itération de la boucle principale (mise à jour du moteur + rendu de la scène)
    audio.Update();
    window.Update();
    movementSystem->Update(coord, dt);
    renderSystem->Render(coord, renderer); // RenderSystem commence par BeginFrame() et dessine la scène (ne présente pas)

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
}

void Engine::Application::Shutdown()
{
    std::cout << "Shutting down application...\n";

    window.ShouldClose();
    isRunning = false;
}

// --- Editor-friendly ECS helpers ---

::Entity Engine::Application::CreateRenderableEntity()
{
    ::Entity e = coord.CreateEntity();

    ::Transform tr;
    tr.position = { 0.0f, 0.0f, 0.0f };
    tr.scale = { 1.0f, 1.0f, 1.0f };
    tr.rotation = { 0.0f, 0.0f, 0.0f, 1.0f };

    coord.AddComponent<::Transform>(e, tr);

    MeshRenderer mr;
    mr.vertexBuffer = renderer.GetMesh().vertexBuffer;
    mr.indexBuffer = renderer.GetMesh().indexBuffer;
    mr.indexCount = renderer.GetMesh().indexCount;
    coord.AddComponent<MeshRenderer>(e, mr);

    // Optionnel : add zero velocity
    Velocity vel;
    vel.velocity = { 0.0f, 0.0f, 0.0f };
    coord.AddComponent<Velocity>(e, vel);

    m_sceneEntities.push_back(e);
    return e;
}

::Transform Engine::Application::GetTransform(::Entity entity)
{
    // Retourne une copie du Transform
    return coord.GetComponent<::Transform>(entity);
}

void Engine::Application::SetTransform(::Entity entity, const ::Transform& t)
{
    coord.GetComponent<::Transform>(entity) = t;
}

// Supprime une entité: la détruit via le coordinator et la retire de la liste utilisée par l'UI
void Engine::Application::DestroyEntity(::Entity entity)
{
    // 1) Détruire dans l'ECS (components, systèmes, recycler l'ID)
    coord.DestroyEntity(entity);

    // 2) Retirer de la liste d'entités de la scène (conserver l'ordre des autres)
    auto it = std::find(m_sceneEntities.begin(), m_sceneEntities.end(), entity);
    if (it != m_sceneEntities.end())
        m_sceneEntities.erase(it);
}