#pragma once
#include "Scene.h"

class Scene4: public Scene
{
protected:
	ExplosionGenerator<Particle>* _explosionGenerator = nullptr;
	GravityGenerator<Particle>* _gravityGenerator = nullptr;
	WindGenerator<Particle>* _windGenerator = nullptr;
	WhirlwindGenerator<Particle>* _whirlwindGenerator = nullptr;
public:
	Scene4() {};
	virtual void init(physx::PxPhysics* physics, physx::PxScene* scene)  override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void handleInput(unsigned char key) override;
};

