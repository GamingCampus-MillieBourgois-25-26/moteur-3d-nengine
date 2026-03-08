#pragma once
/**
 * @file Name.h
 * @brief Component that stores a human-readable label for an entity.
 * @ingroup Components
 */

#include <string>

/**
 * @brief Assigns a display name to an entity.
 *
 * Primarily used by the editor UI to identify entities in scene hierarchies
 * and inspector panels.  It has no effect on gameplay or rendering.
 */
struct Name
{
    std::string value; ///< Human-readable entity name (e.g. "Player", "PointLight_01").
};
