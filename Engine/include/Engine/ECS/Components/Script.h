#pragma once
/**
 * @file Script.h
 * @brief Component that attaches a gameplay script instance to an entity.
 * @ingroup Components
 */

#include "Engine/ScriptAPI.h"
#include <string>

/**
 * @brief Links a named gameplay script class to an entity.
 *
 * The ScriptManager uses @p className to instantiate the correct IScript
 * subclass at runtime (loaded from a DLL or registered directly).
 * The @p instance pointer is managed by the ScriptManager.
 *
 * Lifecycle callbacks (OnCreate, OnUpdate, OnDestroy) are driven by the
 * engine's main loop.
 */
struct Script
{
    std::string className;       ///< Registered script class name, e.g. "PlayerController".
    IScript*    instance = nullptr; ///< Pointer to the live script instance (non-owning view).
};
