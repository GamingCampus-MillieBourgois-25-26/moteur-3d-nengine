#pragma once
#include "Engine/ScriptAPI.h"

extern "C" {

    /**
     * @brief Crée une instance de script à partir de son nom.
     *
     * Fonction exportée par la DLL de gameplay.
     *
     * @param name Nom du script à créer
     * @return Instance du script créé
     */
    __declspec(dllexport)
        IScript* CreateScript(const char* name);

    /**
     * @brief Détruit une instance de script.
     *
     * Fonction exportée par la DLL de gameplay.
     *
     * @param script Script à détruire
     */
    __declspec(dllexport)
        void DestroyScript(IScript* script);
}
