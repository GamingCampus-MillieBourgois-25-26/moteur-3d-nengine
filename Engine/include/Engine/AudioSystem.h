#pragma once
#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include <iostream>

/**
 * @brief Gère le système audio de l'application.
 *
 * Encapsule l'initialisation, la mise à jour et la gestion
 * des événements audio via FMOD.
 */
class AudioSystem {

public:

    /**
     * @brief Initialise le système audio.
     * @return true si l'initialisation a réussi, false sinon
     */
    bool Init();

    /**
     * @brief Charge les banques audio nécessaires.
     */
    void LoadBanks();

    /**
     * @brief Met à jour le système audio.
     */
    void Update();

    /**
     * @brief Arrête le système audio et libère les ressources.
     */
    void Shutdown();

    /**
     * @brief Joue un événement audio à partir de son chemin.
     * @param path Chemin de l'événement FMOD
     */
    void PlayEvent(const char* path);

private:

    FMOD::Studio::System* mStudio = nullptr;
    FMOD::System* mCore = nullptr;
};
