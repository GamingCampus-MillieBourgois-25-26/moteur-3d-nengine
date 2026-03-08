#pragma once
/**
 * @file SceneManager.h
 * @brief Creates, stores and activates game scenes.
 * @ingroup Engine
 */

#include <unordered_map>
#include <memory>
#include <string>
#include "Engine/Scene/Scene.h"

/**
 * @brief Manages the collection of Scenes and tracks the active one.
 *
 * Application owns one SceneManager.  Scenes are created via CreateScene()
 * and identified by name.  Only one scene is "active" at a time;
 * Application::Running() drives the active scene's Update() and Render().
 *
 * Usage:
 * @code
 *  Scene* main = sceneManager.CreateScene("Main", &renderer, &scripts);
 *  sceneManager.SetActiveScene("Main");
 *  Scene* active = sceneManager.GetActiveScene();
 * @endcode
 */
class SceneManager
{
public:
    /**
     * @brief Constructs a new scene, stores it and returns a raw pointer to it.
     * @param name          Unique scene name.
     * @param renderer      Shared renderer passed to the scene.
     * @param scriptManager Shared script manager passed to the scene.
     * @return Non-owning pointer to the created scene.
     */
    Scene* CreateScene(const std::string& name,
                       Renderer*          renderer,
                       ScriptManager*     scriptManager);

    /**
     * @brief Returns the currently active scene, or nullptr if none is set.
     */
    Scene* GetActiveScene();

    /// @brief Const overload of GetActiveScene().
    const Scene* GetActiveScene() const;

    /**
     * @brief Sets the active scene by name.
     * @param name  Name of a previously created scene.
     * @note Does nothing (or asserts in debug) if no scene with that name exists.
     */
    void SetActiveScene(const std::string& name);

    /**
     * @brief Returns a non-owning pointer to the named scene, or nullptr.
     * @param name  Scene identifier.
     */
    Scene* GetScene(const std::string& name);

private:
    /// @brief All scenes keyed by their name.
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;

    Scene* m_activeScene = nullptr; ///< Pointer to the currently active scene.
};
