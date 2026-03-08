#pragma once
/**
 * @file MaterialData.h
 * @brief Component holding material/texture resources for an entity.
 * @ingroup Components
 */

#include "Engine/OBJ/WICTextureLoader.h"

/**
 * @brief Stores the GPU texture resources used to shade a mesh.
 *
 * Currently holds only a diffuse texture view.  Additional slots
 * (normal map, roughness, etc.) can be added here as the engine grows.
 *
 * The shader resource view is created by Renderer::CreateTextureFromFile()
 * and is non-owning (the Renderer manages its lifetime).
 */
struct MaterialData
{
    ID3D11ShaderResourceView* diffuse = nullptr; ///< Diffuse / albedo texture (non-owning).
};
