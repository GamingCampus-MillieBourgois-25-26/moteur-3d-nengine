#include "Engine/OBJLoader.h"

void OBJLoader::loadOBJFile()
{
	// réussir a lire le super fichier obj avec les différentes lignes dans big while 

	file.open("D:/GitHub/G-Tech 2/moteur-3d-nengine/Engine/Assets/OBJ/TestMoteurOBJ.obj"); // ici il faut le chemin du fichier OBJ
	if (!file.is_open()) { std::cerr << "Impossible d'ouvrir le fichier OBJ\n"; return; } // sécurité comme ca au moins on est chill si ca fais du caca

    while (std::getline(file, line))
    {
        std::stringstream ss(line); // c'est comme une machine qui découpe les mots en mode [v] [1.0] [2.0] etc
        std::string prefix;
        ss >> prefix; // ici on vient lire le préfix

        // Position
        if (prefix == "v") // on dit que si ca commence par v alors c'est forcément une position et paf ca fais des chocapics
        {
            Vec3 pos;
            ss >> pos.x >> pos.y >> pos.z; // on lit le reste selon le type de ligne pour avoir les positions inscrites dans le fichiers
            positions.push_back(pos); // on ajoute ces positions dans le vecteur 3 pos.
        }

        // UV
        else if (prefix == "vt") 
        {
            Vec2 uv;
            ss >> uv.u >> uv.v;
            uvs.push_back(uv);
        }

        // Normale
        else if (prefix == "vn")
        {
            Vec3 norm;
            ss >> norm.x >> norm.y >> norm.z;
            normals.push_back(norm);
        }

        // Face
        else if (prefix == "f")
        {
            std::string v1, v2, v3;
            ss >> v1 >> v2 >> v3;

            std::cout << "Face brute : " << v1 << " " << v2 << " " << v3 << "\n";

            // Découpe des index positions, UV et normale
            cutFace(v1);
            cutFace(v2);
            cutFace(v3);

            // récup les bonnes pos/UV/normales, construire les vertices.
            
            FaceIndex index1 = cutFace(v1);
            FaceIndex index2 = cutFace(v2);
            FaceIndex index3 = cutFace(v3);

            uint32_t a = addVertex(index1); 
            uint32_t b = addVertex(index2); 
            uint32_t c = addVertex(index3); 
            
            indices.push_back(a); 
            indices.push_back(b); 
            indices.push_back(c);

            // -> ensuite, créer les buffers DX11, créer l'input layout, charger / compiler les shaders, envoyer les buffers au pipeline, drawIndexed().
        }
    }

    std::cout << "Positions : " << positions.size() << "\n";
    std::cout << "UVs       : " << uvs.size() << "\n";
    std::cout << "Normales  : " << normals.size() << "\n";

    indexCount = indices.size();
}

OBJLoader::FaceIndex OBJLoader::cutFace(std::string sg) {
    std::stringstream ssm(sg);
    std::string a, b, c;

    std::getline(ssm, a, '/'); // -> lis dans le flux jusqu'à rencontrer '/', quand c'est fait, stock tout dans la variable a.
    std::getline(ssm, b, '/');
    std::getline(ssm, c, '/');

    FaceIndex fi;
    fi.pos = std::stoi(a) - 1;
    fi.uv = std::stoi(b) - 1;
    fi.norm = std::stoi(c) - 1;

    std::cout << fi.pos << "pos" << " " << fi.uv << "uv" << " " << fi.norm << "norm" << "\n";

    return fi;
}

uint32_t OBJLoader::addVertex(FaceIndex fix) 
{
    VertexKey key{ fix.pos, fix.uv, fix.norm };  

    // Déjà existant ? / Evite les doublons qui vont tout casser
    auto it = vertexCache.find(key); // cherche si on a déjà créé le vertex pour le triplet (pos, uv, norm)
    if (it != vertexCache.end()) // si on trouve le vertex dans le vecteur vertices alors
        return it->second; // l'index existe déjà donc on return celui deja existant et on sert de la fonction

    // Nouveau vertex 
    Vertex v;
    v.position = positions[fix.pos];
    v.normal = normals[fix.norm];
    v.uv = uvs[fix.uv];

    uint32_t index = vertices.size(); // donne le prochain index libre / taille actuelle du super tabeleau et on stock ce nb dans "index"
    vertices.push_back(v); // donc on ajoute le vertex qu'on vient de créer a notre tableau de vertices
    vertexCache[key] = index; // on l'enregistre dans le "cache" de la map pour que si on trouve de nouveau ce vecteur, on en créer pas d'autre

    return index;
}

void OBJLoader::vertexBufferCreation() {
    bufferDesc.Usage = D3D11_USAGE_DEFAULT; // ce buffer ne sera pas modifier après création
    bufferDesc.ByteWidth = sizeof(Vertex) * vertices.size(); // calcul de la taille totale du buffer pour le GPU // -> ce bloc mémoire va dans la VRAM
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ce buffer devient un vertex buffer 
    bufferDesc.CPUAccessFlags = 0; 
    bufferDesc.MiscFlags = 0;

    InitData.pSysMem = vertices.data(); // donnes à DirectX l’adresse des données en RAM // C’est ce que DirectX va copier dans le buffer GPU
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    hr = m_device->CreateBuffer(&bufferDesc, &InitData, &vertexBuffer); // pas sur de ce que je fais ici 
}

void OBJLoader::indexBufferCreation() {
    bufferDesc.Usage = D3D11_USAGE_DEFAULT; // ce buffer ne sera pas modifier après création
    bufferDesc.ByteWidth = sizeof(uint32_t) * indices.size(); // calcul de la taille totale du buffer pour le GPU // -> ce bloc mémoire va dans la VRAM
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // ce buffer devient un index buffer 
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    InitData.pSysMem = indices.data(); // donnes à DirectX l’adresse des données en RAM // C’est ce que DirectX va copier dans le buffer GPU
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    hr = m_device->CreateBuffer(&bufferDesc, &InitData, &indexBuffer);
}

ID3D11Buffer* OBJLoader::getVertexBuffer() {
    return vertexBuffer;
}

ID3D11Buffer* OBJLoader::getIndexBuffer() {
    return indexBuffer;
}

UINT OBJLoader::getIndexCount() {
    return indexCount;
}

void OBJLoader::setDevice(ID3D11Device* d) {
    m_device = d;
}