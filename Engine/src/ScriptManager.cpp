#include "Engine/ScriptManager.h"
#include <iostream>

ScriptManager::~ScriptManager()
{
    // Détruire toutes les instances créées via CreateInstance()
    // (si tu veux gérer ça automatiquement)
    // Rien à faire ici pour les factories internes.

    if (m_module)
    {
        FreeLibrary(m_module);
        m_module = nullptr;
    }
}

bool ScriptManager::LoadModule(const std::string& path)
{
    m_module = LoadLibraryA(path.c_str());
    if (!m_module)
    {
        std::cout << "[ScriptManager] Failed to load module: " << path << "\n";
        return false;
    }

    m_createFn = (CreateFn)GetProcAddress(m_module, "CreateScript");
    m_destroyFn = (DestroyFn)GetProcAddress(m_module, "DestroyScript");

    if (!m_createFn || !m_destroyFn)
    {
        std::cout << "[ScriptManager] Missing CreateScript or DestroyScript in DLL\n";
        FreeLibrary(m_module);
        m_module = nullptr;
        return false;
    }

    std::cout << "[ScriptManager] Loaded script module: " << path << "\n";
    return true;
}

IScript* ScriptManager::Create(const std::string& name)
{
    // 1) Essayer d'abord les scripts internes (générés automatiquement)
    if (factories.contains(name))
    {
        IScript* s = factories[name]();
        if (s)
        {
            s->OnCreate();
            return s;
        }
    }

    // 2) Sinon, essayer la DLL
    if (!m_createFn)
    {
        std::cout << "[ScriptManager] CreateScript not available for: " << name << "\n";
        return nullptr;
    }

    IScript* script = m_createFn(name.c_str());
    if (script)
        script->OnCreate();

    return script;
}

void ScriptManager::Destroy(IScript* script)
{
    if (!script)
        return;

    script->OnDestroy();

    // 1) Si l'instance vient d'une DLL
    if (m_destroyFn)
    {
        m_destroyFn(script);
        return;
    }

    // 2) Sinon, c'est un script interne → delete normal
    delete script;
}
