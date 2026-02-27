#pragma once
#include <iostream>
#include <bitset>

constexpr std::uint32_t MAX_ENTITIES = 20;
constexpr std::uint32_t MAX_COMPONENTS = 32;

using Entity = std::uint32_t; // pas la peine de faire une classe car on a juste besoin de stocker l'ID. // Entity player = 1; Entity enemy = 2;
using ComponentType = std::uint8_t; // index unique d'un type de composant

// Signature : bitset qui dit quels composants une entite possède (0111)

using Signature = std::bitset<MAX_COMPONENTS>;