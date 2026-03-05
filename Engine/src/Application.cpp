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
    window.Create(800, 600, "My Application");
    input = CreateGLFWInput(window.GetGLFWwindow());

    // Camera bindings
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
    loader.loadOBJFile();

    renderer.Initialize(window.GetGLFWwindow(), 800, 600);

    m_sceneManager.CreateScene("MainScene", &renderer, &scriptManager);
    m_sceneManager.SetActiveScene("MainScene");

    CreateRenderableEntity();

    isRunning = true;
}


void Engine::Application::Update(float dt)
{
    Scene* scene = m_sceneManager.GetActiveScene();   // ✔ CORRECT
    if (!scene) return;

    // ⭐ ÉTAPE 1 : Mettre à jour l'input EN PREMIER
    input->Update();
    window.Update();
    audio.Update();

    // ⭐ ÉTAPE 2 : Traiter les inputs (caméra)
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
    else {
        glfwSetInputMode(window.GetGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    // ⭐ ÉTAPE 3 : Mettre à jour la scène
    scene->Update(dt);

    // ⭐ ÉTAPE 4 : Rendu avec la caméra mise à jour
    scene->Render();
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
    Scene* scene = m_sceneManager.GetActiveScene();
    if (!scene) return UINT32_MAX;  // Sécurité

    ::Entity e = scene->CreateRenderableEntity();  // ⭐ Utiliser la scène !

    // Les composants sont ajoutés via AddComponent() qui utilise la scène
    m_sceneEntities.push_back(e);
    return e;
}

::Transform Engine::Application::GetTransform(::Entity entity)
{
    Scene* scene = m_sceneManager.GetActiveScene();
    if (!scene) return ::Transform();
    return scene->GetTransform(entity);
}

void Engine::Application::SetTransform(::Entity entity, const ::Transform& t)
{
    Scene* scene = m_sceneManager.GetActiveScene();   // ✔ CORRECT
    if (!scene) return;
    scene->SetTransform(entity, t);
}


// Supprime une entité: la détruit via le coordinator et la retire de la liste utilisée par l'UI
void Engine::Application::DestroyEntity(::Entity entity)
{
    Scene* scene = m_sceneManager.GetActiveScene();
    if (!scene) return;

    scene->DestroyEntity(entity);

    // Retirer de la liste d'entités de la scène (conserver l'ordre des autres)
    auto it = std::find(m_sceneEntities.begin(), m_sceneEntities.end(), entity);
    if (it != m_sceneEntities.end())
        m_sceneEntities.erase(it);
}
const std::vector<Entity>& Engine::Application::GetEntities() const
{
    return m_sceneManager.GetActiveScene()->GetEntities();
}
