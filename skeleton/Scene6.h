#pragma once
#include "Scene.h"

class Scene6: public Scene
{
protected:

public:
	Scene6() {};
	virtual void init(physx::PxPhysics* physics, physx::PxScene* scene, physx::PxMaterial* material)  override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void handleInput(unsigned char key) override;
};

