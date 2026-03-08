#pragma once
/**
 * @file ECS_Types.h
 * @brief Fundamental type aliases and constants for the Entity-Component-System.
 * @ingroup ECS
 */

#include <iostream>
#include <bitset>

/// @brief Maximum number of entities that can exist simultaneously.
constexpr std::uint32_t MAX_ENTITIES = 1000;

/// @brief Maximum number of distinct component types that can be registered.
constexpr std::uint32_t MAX_COMPONENTS = 32;

/**
 * @brief Unique identifier for a living entity.
 *
 * An entity is simply an integer ID.
 * Example: Entity player = 1; Entity enemy = 2;
 */
using Entity = std::uint32_t;

/**
 * @brief Unique index assigned to a component type when it is registered.
 *
 * Fits in a single byte (max 255 types, far above MAX_COMPONENTS).
 */
using ComponentType = std::uint8_t;

/**
 * @brief Bitmask describing which components an entity owns.
 *
 * Each bit corresponds to a ComponentType index.
 * Example: 0b101 → owns component 0 (Transform) and component 2 (MeshRenderer).
 */
using Signature = std::bitset<MAX_COMPONENTS>;
