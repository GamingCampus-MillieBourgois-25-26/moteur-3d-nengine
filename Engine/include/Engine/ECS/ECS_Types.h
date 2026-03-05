#pragma once
#include <iostream>
#include <bitset>

/**
 * @brief Nombre maximum d'entités pouvant exister simultanément.
 */
constexpr std::uint32_t MAX_ENTITIES = 20;

/**
 * @brief Nombre maximum de types de composants supportés.
 */
constexpr std::uint32_t MAX_COMPONENTS = 32;

/**
 * @brief Identifiant unique d'une entité.
 *
 * Une entité est simplement un entier, sans logique associée.
 */
using Entity = std::uint32_t;

/**
 * @brief Identifiant unique d'un type de composant.
 *
 * Utilisé pour indexer les bits dans une Signature.
 */
using ComponentType = std::uint8_t;

/**
 * @brief Bitset représentant les composants possédés par une entité.
 *
 * Exemple :
 * - 1 = possède le composant
 * - 0 = ne possède pas le composant
 *
 * Ex : 101 → possède Transform, pas Velocity, possède MeshRenderer.
 */
using Signature = std::bitset<MAX_COMPONENTS>;
