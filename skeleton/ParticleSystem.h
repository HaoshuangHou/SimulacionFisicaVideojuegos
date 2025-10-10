#pragma once
#include <list>

class Particle;
class ParticleGen;

class ParticleSystem {
private:
	std::list<Particle*> _particle;
	std::list<ParticleGen*> _generator;

	void create_model_particle();
public:
	ParticleSystem();
	void update(double dt);

};