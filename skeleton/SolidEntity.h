#pragma once

#include "RenderUtils.hpp"
#include "Entity.h"
#include <PxPhysicsAPI.h>
#include <memory>

class SolidEntity : public Entity
{
private:
    double _lifetime = -1.0f;
    double _timeAlive = 0.0f;

    struct ConstructionParams {
        physx::PxPhysics* physics = nullptr;
        physx::PxScene* scene = nullptr;
        bool dynamic = true;
        physx::PxGeometry* geometry = nullptr;
        double density = 1.0f;
        physx::PxMaterial* material = nullptr;
    };
    ConstructionParams* _params = nullptr;

    void createPhysicsObject(physx::PxPhysics* physics, physx::PxScene* scene,
        bool dynamic, const physx::PxVec3& pos,
        const physx::PxGeometry& geometry, double density,
        physx::PxMaterial* material);

public:
    SolidEntity(physx::PxPhysics* physics, physx::PxScene* scene,
        bool dynamic, const physx::PxVec3& pos,
        const physx::PxGeometry& geometry, double density,
        physx::PxMaterial* material, const Vector4& color);

    SolidEntity(const SolidEntity& other, const Vector3& pos);

    virtual void update(double dt) override;
    virtual void create_renderItem() override;

    void addForce(const Vector3& force);
    void addTorque(const Vector3& torque);

    bool is_alive() const;

    // Getters
    double getMass() const;
    Vector3 getVelocity() const;
    Vector3 getAngularVelocity() const;
    Vector3 getPos() const;
    Vector3 getInertiaTensor() const;

    // Setters
    void setVelocity(const Vector3& vel);
    void setAngularVelocity(const Vector3& angVel);
    void setLifeTime(double lifetime);
    void setInertiaTensor(const Vector3& I);
    void setColor(const Vector4& color);

    physx::PxRigidActor* getActor() const;
    physx::PxShape* getShape() const;
};
