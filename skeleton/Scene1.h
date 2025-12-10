#pragma once
#include "Scene.h"
class Scene1: public Scene
{
private:
	GravityGenerator<Particle>* _gravityGenerator;
public:
	Scene1() {};

	virtual void init(physx::PxPhysics* physics, physx::PxScene* scene) override;
	virtual void handleInput(unsigned char key) override;

};

