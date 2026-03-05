#pragma once
#include <string>
#include <windows.h>
#include "Engine/ScriptAPI.h"

/**
 * @brief Gère le chargement et la création de scripts dynamiques.
 *
 * Permet de charger un module de scripts sous forme de DLL
 * et de créer/détruire des instances de scripts à l'exécution.
 */
class ScriptManager
{
public:
    ScriptManager() = default;
    ~ScriptManager();

    /**
     * @brief Charge un module de scripts dynamique.
     * @param path Chemin vers la DLL du module
     * @return true si le module a été chargé avec succès
     */
    bool LoadModule(const std::string& path);

    /**
     * @brief Crée une instance de script par son nom.
     * @param name Nom du script à créer
     * @return Pointeur vers le script créé
     */
    IScript* Create(const std::string& name);

    /**
     * @brief Détruit une instance de script.
     * @param script Script à détruire
     */
    void Destroy(IScript* script);

private:
    HMODULE m_module = nullptr;

    using CreateFn = IScript * (*)(const char*);
    using DestroyFn = void (*)(IScript*);

    CreateFn m_createFn = nullptr;
    DestroyFn m_destroyFn = nullptr;
};
