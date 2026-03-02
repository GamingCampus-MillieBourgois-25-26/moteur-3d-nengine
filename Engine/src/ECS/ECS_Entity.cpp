#include "Engine/ECS/ECS_Entity.h"

EntityManager::EntityManager()
{
	// On pré-remplit la queue avec tous les IDs disponibles (0 -> MAX_ENTITIES) 
	// Cela permet de distribuer les IDs dans l'ordre et de les recycler plus tard.
	for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) // pré-remplissage de la queue car les entités sont libres
		mAvailableEntities.push(entity);
}

Entity EntityManager::CreateEntity()
{
	// Sécurité : on ne doit jamais dépasser MAX_ENTITIES
	assert(mLivingEntityCount < MAX_ENTITIES);

	Entity id = mAvailableEntities.front(); // ici on donne un ID LIBRE a l'entity 
	mAvailableEntities.pop(); // suppression de l'ID (ID plus disponible)

	++mLivingEntityCount; // on ajoute +1 au compteur d'entity vivantes

	return id;
}

void EntityManager::DestroyEntity(Entity entity)
{
	// permet de verifier si on a bien des entity avant de destroy
	assert(mLivingEntityCount > 0); 
	
	// verifie que l'ID est valide
	assert(entity < MAX_ENTITIES); 

	// On reset la signature pour éviter de garder des composants fantômes
	mSignatures[entity].reset(); 

	// On remet l'ID dans la queue des IDs disponibles
	mAvailableEntities.push(entity); 

	// On décrémente le compteur
	--mLivingEntityCount;
}

void EntityManager::SetSignature(Entity entity, Signature signature)
{
	assert(entity < MAX_ENTITIES); 

	// On stocke la signature
	mSignatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity) const // comme la fonction ne modifie rien alors go mettre un const
{
	assert(entity < MAX_ENTITIES);
	return mSignatures[entity];
}