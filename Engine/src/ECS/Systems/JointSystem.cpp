#include "Engine/ECS/Systems/JointSystem.h"
#include <iostream>

void JointSystem::Init(std::shared_ptr<PhysicsBodySystem> physicsSystem)
{
    m_physicsSystem = physicsSystem;
    std::cout << "JointSystem initialized.\n";
}

void JointSystem::Update(Coordinator& coord)
{
    if (!m_physicsSystem)
        return;

    btDynamicsWorld* world = m_physicsSystem->GetDynamicsWorld();
    if (!world)
        return;

    for (auto entity : mEntities)
    {
        auto& joint = coord.GetComponent<Joint>(entity);

        // Ne creer la contrainte qu'une seule fois
        if (joint.initialized)
            continue;

        // Les deux rigid bodies doivent exister
        btRigidBody* bodyA = m_physicsSystem->GetRigidBody(entity);
        btRigidBody* bodyB = m_physicsSystem->GetRigidBody(joint.targetEntity);

        if (!bodyA || !bodyB)
            continue;

        // Creer la contrainte
        auto constraint = CreateConstraint(joint, bodyA, bodyB);
        if (!constraint)
        {
            std::cout << "JointSystem: failed to create constraint for entity " << entity << "\n";
            continue;
        }

        // Seuil de rupture (0 = incassable)
        if (joint.breakingForce > 0.0f)
            constraint->setBreakingImpulseThreshold(joint.breakingForce);

        // Ajouter la contrainte au monde Bullet
        world->addConstraint(constraint.get(), true);

        JointData data;
        data.constraint = std::move(constraint);
        m_joints[entity] = std::move(data);

        joint.initialized = true;

        std::cout << "JointSystem: created constraint for entity " << entity
                  << " -> " << joint.targetEntity << "\n";
    }
}

std::unique_ptr<btTypedConstraint> JointSystem::CreateConstraint(
    const Joint& joint,
    btRigidBody* bodyA,
    btRigidBody* bodyB)
{
    btVector3 pivotA(joint.pivotA.x, joint.pivotA.y, joint.pivotA.z);
    btVector3 pivotB(joint.pivotB.x, joint.pivotB.y, joint.pivotB.z);

    switch (joint.type)
    {
    case JointType::Point2Point:
    {
        return std::make_unique<btPoint2PointConstraint>(
            *bodyA, *bodyB, pivotA, pivotB);
    }

    case JointType::Hinge:
    {
        btVector3 axisA(joint.axisA.x, joint.axisA.y, joint.axisA.z);
        btVector3 axisB(joint.axisB.x, joint.axisB.y, joint.axisB.z);

        auto hinge = std::make_unique<btHingeConstraint>(
            *bodyA, *bodyB, pivotA, pivotB, axisA, axisB);

        // Appliquer les limites angulaires si definies
        if (joint.lowerLimit != 0.0f || joint.upperLimit != 0.0f)
            hinge->setLimit(joint.lowerLimit, joint.upperLimit);

        return hinge;
    }

    case JointType::Fixed:
    {
        // btFixedConstraint utilise des frames de reference completes
        btTransform frameA;
        frameA.setIdentity();
        frameA.setOrigin(pivotA);

        btTransform frameB;
        frameB.setIdentity();
        frameB.setOrigin(pivotB);

        return std::make_unique<btFixedConstraint>(*bodyA, *bodyB, frameA, frameB);
    }

    default:
        return nullptr;
    }
}

void JointSystem::RemoveJoint(Entity entity)
{
    auto it = m_joints.find(entity);
    if (it == m_joints.end())
        return;

    btDynamicsWorld* world = m_physicsSystem->GetDynamicsWorld();
    if (world)
        world->removeConstraint(it->second.constraint.get());

    m_joints.erase(it);
}

void JointSystem::Shutdown()
{
    btDynamicsWorld* world = m_physicsSystem ? m_physicsSystem->GetDynamicsWorld() : nullptr;

    for (auto& [entity, data] : m_joints)
    {
        if (world)
            world->removeConstraint(data.constraint.get());
    }
    m_joints.clear();

    std::cout << "JointSystem shut down.\n";
}