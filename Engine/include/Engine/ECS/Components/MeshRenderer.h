#pragma once
/**
 * @file MeshRenderer.h
 * @brief Component holding Direct3D 11 GPU buffers for mesh rendering.
 * @ingroup Components
 */

#include <d3d11.h>

/**
 * @brief Stores GPU-side vertex and index buffers required to draw a mesh.
 *
 * The RenderSystem reads this component every frame and submits draw calls
 * via the Renderer.  The buffers are created and owned by the Renderer;
 * this component holds non-owning pointers.
 *
 * @note No geometry data is stored here – all data lives on the GPU.
 */
struct MeshRenderer
{
    ID3D11Buffer* vertexBuffer = nullptr; ///< GPU vertex buffer (non-owning).
    ID3D11Buffer* indexBuffer  = nullptr; ///< GPU index buffer  (non-owning).
    UINT          indexCount   = 0;       ///< Number of indices to draw.
};
