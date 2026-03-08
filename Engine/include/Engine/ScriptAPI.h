#pragma once
/**
 * @file ScriptAPI.h
 * @brief Public scripting interface exposed to gameplay DLLs.
 * @ingroup Engine
 */

/**
 * @brief Context data passed to IScript::OnUpdate() every frame.
 */
struct ScriptContext
{
    float dt; ///< Delta-time in seconds since the last frame.
};

/**
 * @brief Abstract base class for all gameplay scripts.
 *
 * Game modules implement this interface and register their concrete classes
 * with the ScriptManager.  The engine calls the lifecycle methods at the
 * appropriate points:
 *
 *  1. OnCreate()  – called once when the script is first instantiated.
 *  2. OnUpdate()  – called every frame with the current ScriptContext.
 *  3. OnDestroy() – called when the entity owning the script is destroyed.
 *
 * @note All three methods have empty default implementations so scripts
 *       only override what they need.
 */
class IScript
{
public:
    virtual ~IScript() = default;

    /// @brief Called once after the script is created and attached to an entity.
    virtual void OnCreate() {}

    /**
     * @brief Called every frame to update game logic.
     * @param ctx  Frame context containing delta-time.
     */
    virtual void OnUpdate(const ScriptContext& ctx) {}

    /// @brief Called once just before the script is destroyed.
    virtual void OnDestroy() {}
};
