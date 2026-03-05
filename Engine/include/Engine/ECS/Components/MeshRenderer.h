#pragma once
#include <d3d11.h>

/**
 * @brief Composant de rendu de mesh.
 *
 * Contient les buffers nécessaires au rendu d'un mesh
 * sans logique associée.
 */
struct MeshRenderer {
    /** @brief Buffer de sommets du mesh. */
    ID3D11Buffer* vertexBuffer = nullptr;

    /** @brief Buffer d'indices du mesh. */
    ID3D11Buffer* indexBuffer = nullptr;

    /** @brief Nombre d'indices à dessiner. */
    UINT indexCount;
};
