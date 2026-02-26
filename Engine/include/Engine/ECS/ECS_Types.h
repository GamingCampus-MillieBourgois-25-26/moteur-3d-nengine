#pragma once
#include <iostream>

using Entity = std::uint32_t; // pas la peine de faire une classe car on a juste besoin de stocker l'ID. // Entity player = 1; Entity enemy = 2;
using ComponentType = std::uint8_t; // index unique d'un type de composant

constexpr std::uint32_t MAX_ENTITIES = 20;
constexpr std::uint32_t MAX_COMPONENTS = 32;