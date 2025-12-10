#pragma once
#include "Scene.h"
class Scene2: public Scene
{
private:
	Camera* _camera;
	GravityGenerator<Particle>* _gravityGenerator;
public:
	Scene2() {};
	virtual void init(physx::PxPhysics* physics, physx::PxScene* scene) override;
	virtual void handleInput(unsigned char key) override;
	void create_projectil(ProjectilType t, float sim_speed);
};

