#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Engine/Scene/Scene.h"

class SceneManager
{
public:
    Scene* CreateScene(const std::string& name, Renderer* renderer, ScriptManager* scriptManager);

    Scene* GetActiveScene();
    const Scene* GetActiveScene() const;

    void SetActiveScene(const std::string& name);

    Scene* GetScene(const std::string& name);

private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;
    Scene* m_activeScene = nullptr;
};