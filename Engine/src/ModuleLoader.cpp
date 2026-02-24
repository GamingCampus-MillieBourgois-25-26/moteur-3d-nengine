#include "Engine/ModuleLoader.h"
#include <windows.h>
#include <filesystem>
#include <chrono>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

struct ModuleLoader::Impl
{
    HMODULE handle = nullptr;
    std::string originalPath;
    std::string shadowPath;
    fs::file_time_type lastWrite{};

    // Fonction pointer types
    using InitFn = bool(*)();
    using UpdateFn = void(*)(float);
    using ShutdownFn = void(*)();

    InitFn initFn = nullptr;
    UpdateFn updateFn = nullptr;
    ShutdownFn shutdownFn = nullptr;
};

static std::string MakeShadowPath(const std::string& original)
{
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::ostringstream oss;
    // Use filename.hot.<ms>.dll to avoid path collisions
    oss << original << ".hot." << ms << ".dll";
    return oss.str();
}

ModuleLoader::~ModuleLoader()
{
    Unload();
    if (m_impl) { delete m_impl; m_impl = nullptr; }
}

bool ModuleLoader::Load(const std::string& originalPath)
{
    // Unload previous
    Unload();

    try {
        if (!fs::exists(originalPath)) {
            std::cout << "[ModuleLoader] DLL not found: " << originalPath << "\n";
            return false;
        }
    }
    catch (...) {
        std::cout << "[ModuleLoader] Error checking DLL existence: " << originalPath << "\n";
        return false;
    }

    m_impl = new Impl();
    m_impl->originalPath = originalPath;

    // Shadow copy
    m_impl->shadowPath = MakeShadowPath(originalPath);
    try {
        fs::copy_file(originalPath, m_impl->shadowPath, fs::copy_options::overwrite_existing);
    }
    catch (const fs::filesystem_error& e) {
        std::cout << "[ModuleLoader] Failed to copy DLL: " << e.what() << "\n";
        delete m_impl; m_impl = nullptr;
        return false;
    }

    // Load library from shadow copy
    HMODULE mod = LoadLibraryA(m_impl->shadowPath.c_str());
    if (!mod) {
        std::cout << "[ModuleLoader] LoadLibrary failed on " << m_impl->shadowPath << " (GetLastError=" << GetLastError() << ")\n";
        try { fs::remove(m_impl->shadowPath); }
        catch (...) {}
        delete m_impl; m_impl = nullptr;
        return false;
    }

    // Resolve required functions
    FARPROC pInit = GetProcAddress(mod, "GameModule_Init");
    FARPROC pUpdate = GetProcAddress(mod, "GameModule_Update");
    FARPROC pShutdown = GetProcAddress(mod, "GameModule_Shutdown");

    // We accept modules even if Init/Shutdown are missing, but Update is expected for runtime behaviour.
    if (!pUpdate) {
        std::cout << "[ModuleLoader] Required export GameModule_Update not found in " << m_impl->shadowPath << "\n";
        FreeLibrary(mod);
        try { fs::remove(m_impl->shadowPath); }
        catch (...) {}
        delete m_impl; m_impl = nullptr;
        return false;
    }

    m_impl->handle = mod;
    m_impl->initFn = pInit ? reinterpret_cast<Impl::InitFn>(pInit) : nullptr;
    m_impl->updateFn = reinterpret_cast<Impl::UpdateFn>(pUpdate);
    m_impl->shutdownFn = pShutdown ? reinterpret_cast<Impl::ShutdownFn>(pShutdown) : nullptr;

    try {
        m_impl->lastWrite = fs::last_write_time(originalPath);
    }
    catch (...) {
        m_impl->lastWrite = fs::file_time_type::min();
    }

    std::cout << "[ModuleLoader] Loaded module from " << m_impl->shadowPath << "\n";
    return true;
}

void ModuleLoader::Unload()
{
    if (!m_impl) return;

    // Call Shutdown if present
    if (m_impl->shutdownFn) {
        try { m_impl->shutdownFn(); }
        catch (...) {}
    }

    if (m_impl->handle) {
        FreeLibrary(m_impl->handle);
        m_impl->handle = nullptr;
    }

    // remove shadow file if possible
    if (!m_impl->shadowPath.empty()) {
        try { fs::remove(m_impl->shadowPath); }
        catch (...) {}
        m_impl->shadowPath.clear();
    }

    delete m_impl;
    m_impl = nullptr;
}

bool ModuleLoader::IsLoaded() const
{
    return m_impl && m_impl->handle != nullptr;
}

bool ModuleLoader::CallInit()
{
    if (!m_impl || !m_impl->initFn) return true; // no init is considered success
    try {
        return m_impl->initFn();
    }
    catch (...) {
        std::cout << "[ModuleLoader] Exception in module Init\n";
        return false;
    }
}

void ModuleLoader::CallUpdate(float dt)
{
    if (!m_impl || !m_impl->updateFn) return;
    try {
        m_impl->updateFn(dt);
    }
    catch (...) {
        std::cout << "[ModuleLoader] Exception in module Update\n";
    }
}

void ModuleLoader::CallShutdown()
{
    if (!m_impl || !m_impl->shutdownFn) return;
    try {
        m_impl->shutdownFn();
    }
    catch (...) {
        std::cout << "[ModuleLoader] Exception in module Shutdown\n";
    }
}

bool ModuleLoader::ReloadIfChanged()
{
    if (!m_impl) return false;
    try {
        auto current = fs::last_write_time(m_impl->originalPath);
        if (current != m_impl->lastWrite) {
            std::cout << "[ModuleLoader] Change detected for " << m_impl->originalPath << ", reloading...\n";
            std::string path = m_impl->originalPath;
            // preserve shadow copy deletion and call Shutdown inside Unload
            Unload();
            if (Load(path)) {
                // Call Init after reload if present
                if (!CallInit()) {
                    std::cout << "[ModuleLoader] Reloaded module Init returned false\n";
                }
                return true;
            }
            else {
                std::cout << "[ModuleLoader] Reload failed\n";
            }
        }
    }
    catch (...) {
        // ignore transient filesystem errors
    }
    return false;
}

std::filesystem::file_time_type ModuleLoader::GetOriginalWriteTime() const
{
    return m_impl ? m_impl->lastWrite : fs::file_time_type::min();
}