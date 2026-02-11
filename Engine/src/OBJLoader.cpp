#include "Engine/OBJLoader.h"

void OBJLoader::loadOBJFile()
{
	// réussir a lire le super fichier obj avec les différentes lignes dans big while 

	file.open("E:/GitHub Desktop/Repositories/moteur-3d-nengine/Engine/Assets/TestMoteurOBJ.obj"); // ici il faut le chemin du fichier OBJ
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

            // Prochain Objectif : découper et convertir les “1/2/3” en indices utilisables
            // attention aux pièges lors du split & convert car le format OBJ commence à 1, mais C++ commence à 0.
            // donc 4.5.6 deviendra 3.4.5
            // position 0 en c++ = position 1 en obj etc donc en gros on faut tjrs -1.

            // découper, convertir en int, faire -1, récup les bonnes pos/UV/normales, construire les vertices.
            

            // -> ensuite, créer les buffers DX11, créer l'input layout, charger / compiler les shaders, envoyer les buffers au pipeline, drawIndexed().
        }
    }

    std::cout << "Positions : " << positions.size() << "\n";
    std::cout << "UVs       : " << uvs.size() << "\n";
    std::cout << "Normales  : " << normals.size() << "\n";
}
