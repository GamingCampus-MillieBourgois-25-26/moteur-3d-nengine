#include "Engine/OBJ/NewOBJLoader.h"

// Hash custom pour pouvoir utiliser Vertex comme clé dans un unordered_map
struct VertexHash {
    size_t operator()(Vertex const& v) const noexcept {
        // On combine les hash des composantes position/normal/uv
        size_t h1 = std::hash<float>()(v.position.x) ^ (std::hash<float>()(v.position.y) << 1) ^ (std::hash<float>()(v.position.z) << 2);
        size_t h2 = std::hash<float>()(v.normal.x) ^ (std::hash<float>()(v.normal.y) << 1) ^ (std::hash<float>()(v.normal.z) << 2);
        size_t h3 = std::hash<float>()(v.uv.x) ^ (std::hash<float>()(v.uv.y) << 1);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

// Comparateur d'égalité pour Vertex (utilisé par unordered_map)
struct VertexEq {
    bool operator()(Vertex const& a, Vertex const& b) const noexcept {
        return a.position.x == b.position.x &&
            a.position.y == b.position.y &&
            a.position.z == b.position.z &&
            a.normal.x == b.normal.x &&
            a.normal.y == b.normal.y &&
            a.normal.z == b.normal.z &&
            a.uv.x == b.uv.x &&
            a.uv.y == b.uv.y;
    }
};

MeshData LoadOBJ(const std::string& path) {
    // Lecteur tinyobjloader
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig config;

    // Où chercher les fichiers .mtl (matériaux)
    config.mtl_search_path = "./"; 

    // Parse du fichier .obj
    if (!reader.ParseFromFile(path, config)) {
        // En cas d'erreur, on récupère le message de tinyobjloader
        std::string err = reader.Error(); 
        if (err.empty()) { err = "Unknown error while parsing OBJ file."; }
        throw std::runtime_error("TinyObjLoader error: " + err);
    }

    // Affichage des warnings éventuels (non bloquants) 
    if (!reader.Warning().empty()) { std::cout << "TinyObjLoader warning: " << reader.Warning() << std::endl; }

    // Attributs globaux : positions, normales, UVs
    const auto& attrib = reader.GetAttrib();

    // Shapes : chaque shape contient des indices vers ces attributs
    const auto& shapes = reader.GetShapes();

    // Matériaux (non utilisés ici, mais disponibles) 
    //const auto& materials = reader.GetMaterials();

    MeshData mesh;

    // Map pour éviter les doublons de vertices 
    // clé : Vertex complet (pos+normal+uv) 
    // valeur: index dans mesh.vertices
    std::unordered_map<Vertex, uint32_t, VertexHash, VertexEq> uniqueVertices;

    // On parcourt chaque shape (souvent un sous-mesh)
    for (const auto& shape : shapes) {
        // shape.mesh.indices contient la liste des indices (vertex/normal/uv)
        for (const auto& index : shape.mesh.indices) {
            Vertex v{};

            // --- Position --- 
            // index.vertex_index est un index dans attrib.vertices (tableau de float) 
            // Chaque position = 3 floats (x, y, z)
            v.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            // --- Normale ---
            // index.normal_index est un index dans attrib.normals (3 floats par normale)
            if (index.normal_index >= 0) {
                v.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }
            else { 
                // Optionnel : tu peux mettre une normale par défaut (0,0,0) ou (0,1,0)
                v.normal = { 0.0f, 0.0f, 0.0f }; 
            }

            // --- Coordonnées UV ---
            // index.texcoord_index est un index dans attrib.texcoords (2 floats par UV)
            if (index.texcoord_index >= 0) {
                float u = attrib.texcoords[2 * index.texcoord_index + 0];
                float vTex = attrib.texcoords[2 * index.texcoord_index + 1];

                // On flip l'axe Y des UV (souvent nécessaire selon la convention du moteur)
                v.uv = { u, 1.0f - vTex };
            }
            else {
                // UV par défaut si non présents
                v.uv = { 0.0f, 0.0f };
            }

            // --- Déduplication des vertices ---
            // Si ce vertex n'existe pas encore, on l'ajoute
            if (uniqueVertices.count(v) == 0) {
                uint32_t newIndex = static_cast<uint32_t>(mesh.vertices.size());               
                uniqueVertices[v] = newIndex;
                mesh.vertices.push_back(v);
            }

            // On ajoute l'index correspondant dans le buffer d'indices
            mesh.indices.push_back(uniqueVertices[v]);
        }
    }

    return mesh;
}
