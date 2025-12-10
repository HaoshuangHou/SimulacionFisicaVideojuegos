#pragma once
#include "SolidSystem.h"
#include "GravityGenerator.h"
#include "WindGenerator.h"
#include "NormalDistributionGen.h"

class BubbleSystem : public SolidSystem
{
private:
public:
    BubbleSystem(const Vector3& pos, physx::PxPhysics* physics, physx::PxScene* scene)
        : SolidSystem(physics, scene, pos, 30.0f)
    {
        physx::PxMaterial* bubbleMaterial = physics->createMaterial(0.01f, 0.01f, 0.5f);

        _model_solid = new SolidEntity(
            physics, scene, true,
            Vector3(0, 0, 0),
            physx::PxSphereGeometry(10.0f),
            1.5f, 
            bubbleMaterial, 
            Vector4(0.8f, 0.9f, 1.0f, 0.7f)
        );

        NormalDistributionGen<SolidEntity>* gen =
            new NormalDistributionGen<SolidEntity>(
                _model_solid,
                Vector3(0, 0, 0),    
                Vector3(0, 10.0f, 0),  
                20.0f, 
                2  
            );

        gen->setDesPos(Vector3(0.5f, 0.5f, 0.5f)); 
        gen->setDesVel(Vector3(0.2f, 0.1f, 0.2f));

        addGenerator(gen);

        addForce(new GravityGenerator<SolidEntity>(Vector3(0, -9.8f, 0)));

        addForce(new WindGenerator<SolidEntity>(
            Vector3(0, 5, 0),  // Centro
            25.0f,             // Radio
            Vector3(1.0f, 0, 0), // Viento hacia +X
            0.3f,              // k1
            0.05f              // k2
        ));
    }

private:
  
};