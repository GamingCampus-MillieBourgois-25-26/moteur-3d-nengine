#include "Engine/ScriptManager.h"
#include <iostream>

ScriptManager::~ScriptManager()
{
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
    if (!m_createFn)
    {
        std::cout << "[ScriptManager] CreateScript not available\n";
        return nullptr;
    }

    return m_createFn(name.c_str());
}

void ScriptManager::Destroy(IScript* script)
{
    if (!script || !m_destroyFn)
        return;

    m_destroyFn(script);
}
