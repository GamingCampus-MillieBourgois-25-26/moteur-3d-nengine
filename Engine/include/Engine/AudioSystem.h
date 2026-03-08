#pragma once
/**
 * @file AudioSystem.h
 * @brief FMOD Studio audio system wrapper.
 * @ingroup Engine
 */

#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include <iostream>

/**
 * @brief Manages FMOD Studio initialisation, bank loading and event playback.
 *
 * AudioSystem wraps the FMOD Studio API (high-level event system) and the
 * FMOD Core API (low-level mixing engine).  A single instance is owned by
 * Application and updated once per frame.
 *
 * Typical usage:
 * @code
 *  audio.Init();
 *  audio.LoadBanks();   // loads .bank files from disk
 *  // … game loop …
 *  audio.PlayEvent("event:/SFX/Explosion");
 *  audio.Update();      // must be called every frame
 *  audio.Shutdown();
 * @endcode
 */
class AudioSystem
{
public:
    /**
     * @brief Initialises the FMOD Studio system and the low-level core system.
     * @return true on success, false if FMOD initialisation failed.
     */
    bool Init();

    /**
     * @brief Loads all FMOD Studio .bank files required by the project.
     *
     * Banks contain event definitions, sample data and mixer snapshots.
     * Call this after Init() and before the first PlayEvent().
     */
    void LoadBanks();

    /**
     * @brief Processes FMOD's internal command queue (call once per frame).
     *
     * Must be called every frame for streaming, voice management and
     * callback dispatch to function correctly.
     */
    void Update();

    /**
     * @brief Releases all FMOD resources and shuts down both systems.
     */
    void Shutdown();

    /**
     * @brief Starts playback of an FMOD Studio event.
     * @param path  FMOD event path, e.g. "event:/SFX/Footstep".
     *
     * Creates a one-shot event instance and immediately starts it.
     * The instance is released automatically when playback finishes.
     */
    void PlayEvent(const char* path);

private:
    FMOD::Studio::System* mStudio = nullptr; ///< FMOD Studio high-level system.
    FMOD::System*         mCore   = nullptr; ///< FMOD Core low-level system.
};
