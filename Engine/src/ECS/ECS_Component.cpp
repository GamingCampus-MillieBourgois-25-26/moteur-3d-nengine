#include "Engine/ECS/ECS_Component.h"

template<typename T>
void ComponentArray<T>::InsertData(Entity entity, T component)
{
    assert(mSize < MAX_ENTITIES && "Too many components of this type.");
    assert(mEntityToIndex.find(entity) == mEntityToIndex.end());

    size_t newIndex = mSize;

    mEntityToIndex[entity] = newIndex;
    mIndexToEntity[newIndex] = entity;
    mComponentArray[newIndex] = component;

    ++mSize;
}

template<typename T>
void ComponentArray<T>::RemoveData(Entity entity)
{
    assert(mEntityToIndex.find(entity) != mEntityToIndex.end());

    size_t indexOfRemovedEntity = mEntityToIndex[entity];
    size_t indexOfLastElement = mSize - 1;

    // Déplacer le dernier élément
    mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

    Entity entityOfLastElement = mIndexToEntity[indexOfLastElement];

    mEntityToIndex[entityOfLastElement] = indexOfRemovedEntity;
    mIndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

    // Supprimer les anciennes entrées
    mEntityToIndex.erase(entity);
    mIndexToEntity.erase(indexOfLastElement);

    --mSize;
}

template<typename T>
T& ComponentArray<T>::GetData(Entity entity)
{
    assert(mEntityToIndex.find(entity) != mEntityToIndex.end());

    return mComponentArray[mEntityToIndex[entity]];
}

template<typename T>
void ComponentArray<T>::EntityDestroyed(Entity entity)
{
    if (mEntityToIndex.find(entity) != mEntityToIndex.end())
    {
        RemoveData(entity);
    }
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Component manager ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void ComponentManager::RegisterComponent()
{
    std::type_index typeName(typeid(T));

    // Vérifie si le composant est déjà enregistré
    assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Component type already registered.");

    // Assigne un ID unique
    mComponentTypes[typeName] = mNextComponentType;

    // Crée et stocke le ComponentArray<T>
    mComponentArrays[typeName] = std::make_unique<ComponentArray<T>>();

    ++mNextComponentType;
    assert(mNextComponentType <= MAX_COMPONENTS && "Exceeded maximum number of components.");
}

template<typename T>
void ComponentManager::AddComponent(Entity entity, T component)
{
    std::type_index typeName(typeid(T));

    auto componentArray = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
    assert(componentArray && "Component not registered.");

    componentArray->InsertData(entity, component);
}

template<typename T>
T& ComponentManager::GetComponent(Entity entity)
{
    std::type_index typeName(typeid(T));

    //auto componentArray = std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
    auto componentArray = static_cast<ComponentArray<T>*>(mComponentArrays[typeName].get());
    assert(componentArray && "Component not registered.");

    return componentArray->GetData(entity);
}

template<typename T>
ComponentType ComponentManager::GetComponentType()
{
    std::type_index typeName(typeid(T));
    assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered.");

    return mComponentTypes[typeName];
}

void ComponentManager::EntityDestroyed(Entity entity)
{
    for (auto const& pair : mComponentArrays)
    {
        auto const& componentArray = pair.second;
        componentArray->EntityDestroyed(entity);
    }
}