#pragma once
#include <list>
#include <memory>
#include "core.hpp"
#include "Generator.h"

class Particle;
class ForceGenerator;
class ForceRegistry;

class ParticleSystem {
protected:
	Vector3 _center;
	float _radius;
	Vector4 _startColor;
	Vector4 _endColor;

	std::list<std::unique_ptr<Particle>> _particles;
	std::list<Generator<Particle>*> _generators;
	std::list<std::unique_ptr<ForceGenerator>> _forces;
	std::unique_ptr<ForceRegistry> _forceRegistry;

	Particle* _model_particle;
	void create_model_particle();
	void create_model_particle(double tam);
	virtual void delete_particle();

	bool isParticleOutsideArea(Particle* particle);

public:
	ParticleSystem(const Vector3& center, float r);
	virtual ~ParticleSystem();
	virtual void update(double dt);
	void addGenerator(Generator<Particle>* gen);
	void addForce(ForceGenerator* f);

	void registerAllRenderItems();
	void deregisterAllRenderItems();
};