#pragma once
/**
 * @file ScriptManager.h
 * @brief Loads gameplay script modules and instantiates IScript objects.
 * @ingroup Engine
 */

#include <string>
#include <windows.h>
#include "Engine/ScriptAPI.h"
#include <unordered_map>
#include <functional>

/**
 * @brief Manages the lifecycle of gameplay scripts loaded from DLLs.
 *
 * Supports two complementary workflows:
 *
 * **In-process registration** (editor / test builds):
 * @code
 *  scriptManager.RegisterScript<PlayerController>("PlayerController");
 *  IScript* s = scriptManager.CreateInstance("PlayerController");
 * @endcode
 *
 * **DLL-based loading** (runtime / shipped builds):
 * @code
 *  scriptManager.LoadModule("GameModule.dll");
 *  IScript* s = scriptManager.Create("PlayerController");
 *  // …
 *  scriptManager.Destroy(s);
 * @endcode
 */
class ScriptManager
{
public:
    ScriptManager() = default;

    /**
     * @brief Unloads the gameplay module (if any) and cleans up.
     */
    ~ScriptManager();

    /**
     * @brief Registers a concrete script type using an in-process factory.
     *
     * The factory simply calls `new T()`.  Use this variant when the script
     * classes are compiled into the same binary as the engine (editor builds).
     *
     * @tparam T    Concrete IScript subclass to register.
     * @param name  Key used to create instances later (e.g. "PlayerController").
     */
    template<typename T>
    void RegisterScript(const std::string& name)
    {
        factories[name] = []() { return new T(); };
    }

    /**
     * @brief Creates an IScript instance via the in-process factory map.
     * @param name  Registered script name.
     * @return New instance, or nullptr if @p name is unknown.
     */
    IScript* CreateInstance(const std::string& name)
    {
        if (factories.contains(name))
            return factories[name]();
        return nullptr;
    }

    /**
     * @brief Loads a gameplay DLL that exports CreateScript / DestroyScript.
     * @param path  Path to the DLL (e.g. "GameModule.dll").
     * @return true on success, false if the module or its exports are missing.
     */
    bool LoadModule(const std::string& path);

    /**
     * @brief Creates a script via the DLL's exported factory.
     * @param name  Script class name as recognised by the DLL.
     * @return New IScript instance owned by the caller, or nullptr.
     */
    IScript* Create(const std::string& name);

    /**
     * @brief Destroys a script created with Create() via the DLL's destructor.
     * @param script  Pointer to the script to destroy.
     */
    void Destroy(IScript* script);

private:
    HMODULE m_module = nullptr; ///< Handle to the loaded gameplay DLL.

    using CreateFn  = IScript* (*)(const char*); ///< DLL export: allocates a script.
    using DestroyFn = void     (*)(IScript*);    ///< DLL export: deallocates a script.

    CreateFn  m_createFn  = nullptr; ///< Pointer to the DLL's create function.
    DestroyFn m_destroyFn = nullptr; ///< Pointer to the DLL's destroy function.

    /// @brief In-process factory map (used by RegisterScript / CreateInstance).
    std::unordered_map<std::string, std::function<IScript*()>> factories;
};
