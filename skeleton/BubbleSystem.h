#pragma once
#include "SolidSystem.h"
#include "GravityGenerator.h"
#include "WindGenerator.h"
#include "WhirlwindGenerator.h"
#include "SolidNormalDistributionGen.h"

class BubbleSystem : public SolidSystem
{
public:
    BubbleSystem(const Vector3& pos, physx::PxPhysics* physics, physx::PxScene* scene, float tam = 4.0, float radius = 100.0f)
        : SolidSystem(physics, scene, pos, radius)
    {
        physx::PxMaterial* bubbleMaterial = physics->createMaterial(0.1f, 0.1f, 0.2f);

        _model_solid = new SolidEntity(
            physics, scene, true,
            pos,
            physx::PxSphereGeometry(tam),
            0.1f, 
            bubbleMaterial, 
            Vector4(0.8f, 0.9f, 1.0f, 0.5f)
        );
        _model_solid->setLifeTime(20.0f);

        SolidNormalDistributionGen* gen =
            new SolidNormalDistributionGen(
                _model_solid,
                pos,
                Vector3(1.0f,2.0f,1.0f),  
                20.0f, 
                2
            );

        gen->setSpawnProbability(0.02);
        gen->setDesPos(Vector3(radius / 4, radius / 8, radius / 4));
        gen->setDesVel(Vector3(1.0f, 1.0f, 1.0f)); 
        gen->setDesInertia(Vector3(0.2f, 0.2f, 0.2f));
        gen->setDesDur(1.0f);

        addGenerator(gen);

       // addForce(new GravityGenerator<SolidEntity>(Vector3(0, 2.0f, 0)));
        addForce(new WhirlwindGenerator<SolidEntity>(pos, 50.0f, 5.0f, 0.1, 0.02));
    }
};