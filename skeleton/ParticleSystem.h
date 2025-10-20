#pragma once
#include <list>

class Particle;
class ParticleGen;

class ParticleSystem {
private:
	std::list<Particle*> _particles;
	std::list<ParticleGen*> _generators;

	Particle* _model_particle;
	void create_model_particle();
protected:
	virtual void delete_particle();
public:
	ParticleSystem();
	void update(double dt);

};