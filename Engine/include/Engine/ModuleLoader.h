#pragma once
#include <string>
#include <filesystem>
#include <cstdint>

/*
 ModuleLoader simple : charge un module DLL qui expose :
   extern "C" __declspec(dllexport) bool GameModule_Init();
   extern "C" __declspec(dllexport) void GameModule_Update(float dt);
   extern "C" __declspec(dllexport) void GameModule_Shutdown();

 Utilise une shadow copy pour permettre la recompilation du DLL sans verrouillage.
*/
class ModuleLoader
{
public:
    ModuleLoader() = default;
    ~ModuleLoader();

    // Charge la DLL originale (chemin vers le .dll produit par le build).
    // Retourne true si la DLL est chargée et que les fonctions requises sont trouvées.
    bool Load(const std::string& originalPath);

    // Décharge proprement la DLL (appelle Shutdown si présent).
    void Unload();

    // Indique si un module est chargé.
    bool IsLoaded() const;

    // Appelle Init sur le module (si présent). Retourne false si absent ou si Init retourne false.
    bool CallInit();

    // Appelle Update sur le module (si présent).
    void CallUpdate(float dt);

    // Appelle Shutdown sur le module (si présent).
    void CallShutdown();

    // Si le fichier original a été modifié, tente de recharger le module.
    // Retourne true si un nouveau module a été chargé.
    bool ReloadIfChanged();

    // Retourne la timepoint de la dernière écriture du fichier original.
    std::filesystem::file_time_type GetOriginalWriteTime() const;

private:
    struct Impl;
    Impl* m_impl = nullptr;
};