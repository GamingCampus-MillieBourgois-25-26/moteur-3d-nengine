#pragma once
#include <iostream>
#include "MathsLib/Quaternion.h"
#include "MathsLib/Vector3.h"
#include <queue>
#include <map>
#include <thread>
#include <bitset>

// Composant : un composant est un struct simple avec seuelement des valeurs sans qu'elles soient definies (pure)

using ComponentType = std::uint8_t; // index unique d'un type de composant
constexpr std::uint32_t MAX_COMPONENTS = 32;

class IComponentArray // Le ComponentManager doit pouvoir appeler EntityDestroyed() sans connaître le type T.
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
    void InsertData(Entity entity, T component);
    void RemoveData(Entity entity);
    T& GetData(Entity entity);

    void EntityDestroyed(Entity entity) override;

private:
    std::array<T, MAX_ENTITIES> mComponentArray;
    std::unordered_map<Entity, size_t> mEntityToIndex;
    std::unordered_map<size_t, Entity> mIndexToEntity;
    size_t mSize = 0;
};

/*

Explication importante

On garde :

Tableau dense mComponentArray

Map Entity -> index

Map index -> Entity

mSize = nombre d’elements valides

Quand on supprime un composant :

On remplace la case supprimee par le dernier element

On met à jour les maps

On decremente mSize

-> Aucun trou en memoire.

*/

class ComponentManager // Role : associer un type C++ a un ID de composant, gerer les ComponentArray
{
public:
    template<typename T>
    void RegisterComponent(); // assigne un ID unique (0,1,2), cree un ComponentArray<Transform>

    template<typename T>
    void AddComponent(Entity entity, T component);

    template<typename T>
    T& GetComponent(Entity entity);

    template<typename T>
    ComponentType GetComponentType();

    void EntityDestroyed(Entity entity);

private:
    std::unordered_map<const char*, ComponentType> mComponentTypes;
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays;

    ComponentType mNextComponentType = 0;
};


// Definition d'un composant
struct Transform { MathsLib::Vector3<float> position; MathsLib::Quaternion<float> rotation; MathsLib::Vector3<float> scale; };
struct Velocity { MathsLib::Vector3<float> velocity; };
struct Health { int hp; };