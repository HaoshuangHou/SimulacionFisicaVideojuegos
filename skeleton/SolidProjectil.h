#pragma once
#include "SolidEntity.h"
class SolidProjectil: public SolidEntity
{
public:
    virtual Type getType() const override { return PROJECTILE; }
    SolidProjectil(physx::PxPhysics* physics, physx::PxScene* scene, const Vector3& pos, const Vector3& sim_velocity, Vector4 const& color = Vector4(1, 1, 1, 1), physx::PxU32 sceneGroup = 1);
};

