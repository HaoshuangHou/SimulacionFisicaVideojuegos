#pragma once
#include "Scene.h"
#include <vector>
#include "ElasticBandForceGenerator.h"
#include "SpringForceGenerator.h"

class Scene5: public Scene
{
protected:
	GravityGenerator<Particle>* _gravityGenerator = nullptr;
	ElasticBandForceGenerator<Particle>* _fElastic1 = nullptr;
	ElasticBandForceGenerator<Particle>* _fElastic2 = nullptr;
	SpringForceGenerator<Particle>* _fSpring = nullptr;
	Particle* pEstatico;
	Particle* p1_muelle;
	Particle* p1_goma;
	Particle* p2_goma;
	Particle* pCubo;

	std::vector<Particle*> _slinky_particles;

	void create_slinky();
public:
	Scene5() {};
	virtual void init(physx::PxPhysics* physics, physx::PxScene* scene)  override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void handleInput(unsigned char key) override;
};

