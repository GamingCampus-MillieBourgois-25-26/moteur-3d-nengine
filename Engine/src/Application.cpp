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
    // ... setup audio, loader, etc ...

    if (!renderer.Initialize(window.GetGLFWwindow(), 800, 600))
    {
        std::cout << "Failed to initialize renderer\n";
        return;
    }

    // ⭐ CRÉER ET INITIALISER LA SCÈNE
    Scene* mainScene = m_sceneManager.CreateScene("MainScene", &renderer, &scriptManager);
    m_sceneManager.SetActiveScene("MainScene");

    // ⭐ CRÉER UNE ENTITÉ INITIALE
    Entity e = CreateRenderableEntity();

    // Ajouter les composants
    tr.position = { 0, 0, 0 };
    tr.scale = { 0.5f, 0.5f, 0.5f };
    tr.rotation = { 0, 0, 0, 1 };
    AddComponent<::Transform>(e, tr);

    mr.vertexBuffer = renderer.GetMesh().vertexBuffer;
    mr.indexBuffer = renderer.GetMesh().indexBuffer;
    mr.indexCount = renderer.GetMesh().indexCount;
    AddComponent<MeshRenderer>(e, mr);

    vel.velocity = { 0, 0, 0 };
    AddComponent<Velocity>(e, vel);

    name.value = "Entity";
    AddComponent<Name>(e, name);

    script.className = "Script";
    AddComponent<Script>(e, script);

    m_sceneEntities.push_back(e);

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

    coord.AddComponent<::Transform>(e, tr);

    mr.vertexBuffer = renderer.GetMesh().vertexBuffer;
    mr.indexBuffer = renderer.GetMesh().indexBuffer;
    mr.indexCount = renderer.GetMesh().indexCount;
    coord.AddComponent<MeshRenderer>(e, mr);

    // Optionnel : add zero velocity
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

const std::vector<Entity>& Engine::Application::GetEntities() const 
{
    return m_sceneEntities;
}
