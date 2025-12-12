#pragma once
#include "SolidSystem.h"
#include "GravityGenerator.h"
#include "WindGenerator.h"
#include "NormalDistributionGen.h"
#include "UniformDistributionGen.h"

class BubbleSystem : public SolidSystem
{
public:
    BubbleSystem(const Vector3& pos, physx::PxPhysics* physics, physx::PxScene* scene)
        : SolidSystem(physics, scene, pos, 100.0f)
    {
        physx::PxMaterial* bubbleMaterial = physics->createMaterial(0.1f, 0.1f, 0.2f);

        _model_solid = new SolidEntity(
            physics, scene, true,
            pos,
            physx::PxSphereGeometry(4.0f),
            0.5f, 
            bubbleMaterial, 
            Vector4(0.8f, 0.9f, 1.0f, 0.5f)
        );
        _model_solid->setLifeTime(-1.0f);

        NormalDistributionGen<SolidEntity>* gen =
            new NormalDistributionGen<SolidEntity>(
                _model_solid,
                pos,
                Vector3(0,15.0f,0),  
                -1.0f, 
                1
            );

        gen->setSpawnProbability(0.08);
        gen->setDesPos(Vector3(20.0f, 4.0f, 20.0f));
        gen->setDesVel(Vector3(0.5f, 2.0f, 0.5f)); 
        gen->setDesMass(0.01f);

        addGenerator(gen);

        addForce(new GravityGenerator<SolidEntity>(Vector3(0, 1.0f, 0)));
    }
};