#include "SolidProjectil.h"
#include <math.h> 
#include <algorithm> 
#include <iostream> 

#define _real_speed 30
#define _real_mass 0.2

SolidProjectil::SolidProjectil(physx::PxPhysics* physics, physx::PxScene* scene, const Vector3& pos,
    const Vector3& sim_velocity, Vector4 const& color)
    :SolidEntity(physics, scene, true, pos, physx::PxSphereGeometry(1.0f), 1.0f, 
        physics->createMaterial(0.1f, 0.5f, 2.0f), color)
{
    
    double sim_speed = sim_velocity.magnitude();
    if (sim_speed < 0.1) sim_speed = 0.1;
    
    if (physx::PxRigidDynamic* dyn = _actor->is<physx::PxRigidDynamic>())
    {
        dyn->setMass((_real_mass * _real_speed * _real_speed) / (sim_speed * sim_speed));
        dyn->setLinearVelocity(sim_velocity);

       
    }
}

void SolidProjectil::update(double t)
{
    SolidEntity::update(t);
}
