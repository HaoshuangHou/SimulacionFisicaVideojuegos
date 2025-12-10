#pragma once
#include "Scene.h"
#include "FireworkParticleSystem.h"

class Scene3: public Scene
{
private:
	FireworkParticleSystem* f;
public:
	Scene3() {};
	virtual void init(physx::PxPhysics* physics, physx::PxScene* scene)  override;
	virtual void handleInput(unsigned char key) override;
	void createParticleSystem();

};

