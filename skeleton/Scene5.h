#pragma once
#include "Scene.h"
#include <vector>

class GravityGenerator;
class SpringForceGenerator;
class ElasticBandForceGenerator;

class Scene5: public Scene
{
protected:
	GravityGenerator* _gravityGenerator = nullptr;
	ElasticBandForceGenerator* _fElastic1 = nullptr;
	ElasticBandForceGenerator* _fElastic2 = nullptr;
	SpringForceGenerator* _fSpring = nullptr;
	Particle* pEstatico;
	Particle* p1_muelle;
	Particle* p1_goma;
	Particle* p2_goma;
	Particle* pCubo;

	std::vector<Particle*> _slinky_particles;

	void create_slinky();
public:
	Scene5() {};
	virtual void init() override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void handleInput(unsigned char key) override;
};

