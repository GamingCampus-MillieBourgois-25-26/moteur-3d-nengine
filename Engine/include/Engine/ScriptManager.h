#pragma once
#include <string>
#include <windows.h>
#include "Engine/ScriptAPI.h"
#include <unordered_map>
#include <functional>


class ScriptManager
{
public:
    ScriptManager() = default;
    ~ScriptManager();

    template<typename T>
    void RegisterScript(const std::string& name) {
        factories[name] = []() { return new T(); };
    }

    IScript* CreateInstance(const std::string& name) {
        if (factories.contains(name))
            return factories[name]();
        return nullptr;
    }


    // Charge la DLL gameplay (GameModule.dll)
    bool LoadModule(const std::string& path);

    // Crée un script par son nom (ex: "PlayerController")
    IScript* Create(const std::string& name);

    // Détruit un script créé par Create()
    void Destroy(IScript* script);

private:
    HMODULE m_module = nullptr;

    using CreateFn = IScript * (*)(const char*);
    using DestroyFn = void (*)(IScript*);

    CreateFn m_createFn = nullptr;
    DestroyFn m_destroyFn = nullptr;

    std::unordered_map<std::string, std::function<IScript* ()>> factories;
};
