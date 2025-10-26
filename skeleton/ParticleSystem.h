#pragma once
#include <list>
#include "core.hpp"

class Particle;
class ParticleGen;

class ParticleSystem {
protected:
	Vector3 _center;
	float _radius;
	Vector4 _startColor;
	Vector4 _endColor;

	std::list<Particle*> _particles;
	std::list<ParticleGen*> _generators;

	Particle* _model_particle;
	void create_model_particle();
	void create_model_particle(double tam);
	virtual void delete_particle();

	bool isParticleOutsideArea(Particle* particle);

public:
	ParticleSystem(const Vector3& center, float r);
	virtual ~ParticleSystem();
	virtual void update(double dt);
	void addGenerator(ParticleGen* gen);

	void registerAllRenderItems();
	void deregisterAllRenderItems();
};