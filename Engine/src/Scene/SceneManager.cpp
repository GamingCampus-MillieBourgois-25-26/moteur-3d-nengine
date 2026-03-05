#include "Engine/Scene/SceneManager.h"

Scene* SceneManager::CreateScene(const std::string& name, Renderer* renderer, ScriptManager* scriptManager)
{
    auto scene = std::make_unique<Scene>(name, renderer, scriptManager);
    Scene* ptr = scene.get();
    m_scenes[name] = std::move(scene);
    if (!m_activeScene) m_activeScene = ptr;
    return ptr;
}

Scene* SceneManager::GetActiveScene()
{
    return m_activeScene;
}

void SceneManager::SetActiveScene(const std::string& name)
{
    auto it = m_scenes.find(name);
    if (it != m_scenes.end())
        m_activeScene = it->second.get();
}

Scene* SceneManager::GetScene(const std::string& name)
{
    auto it = m_scenes.find(name);
    return (it != m_scenes.end()) ? it->second.get() : nullptr;
}