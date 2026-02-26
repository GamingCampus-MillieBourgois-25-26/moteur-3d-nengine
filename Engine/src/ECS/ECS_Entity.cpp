#include "Engine/ECS/ECS_Entity.h"

EntityManager::EntityManager()
{
	for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) // pré-remplissage de la queue car les entités sont libres
		mAvailableEntities.push(entity);
}

Entity EntityManager::CreateEntity()
{

	/*
	
	queue.push(x);     // ajoute à la fin
	queue.front();     // regarde l’élément au début
	queue.pop();       // supprime l’élément au début
	queue.empty();     // vérifie si vide
	
	*/

	assert(mLivingEntityCount < MAX_ENTITIES); // marche comme un if mais c'est plus securiser dans un moteur

	Entity id = mAvailableEntities.front(); // ici on donne l'id a l'entity 
	mAvailableEntities.pop(); // suppression de l'ID (ID plus disponible)

	++mLivingEntityCount; // on ajoute +1 au compteur 

	return id;
}

void EntityManager::DestroyEntity(Entity entity)
{
	assert(mLivingEntityCount > 0); // permet de verifier si on a bien des entity avant de destroy
	assert(entity < MAX_ENTITIES); // ici, verifie que l'entity est valide

	mSignatures[entity].reset(); // reinitialisation de la signature pour eviter d'en avoir 8000

	mAvailableEntities.push(entity); 
	--mLivingEntityCount;
}

void EntityManager::SetSignature(Entity entity, Signature signature)
{
	assert(entity < MAX_ENTITIES); // verifie que l'ID est valide
	mSignatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity) const // comme la fonction ne modifie rien alors go mettre un const
{
	assert(entity < MAX_ENTITIES);
	return mSignatures[entity];
}