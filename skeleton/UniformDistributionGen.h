#pragma once
#include "ParticleGen.h"

class Particle;
class UniformDistributionGen : public ParticleGen
{
private:
	std::uniform_real_distribution<double> _uniform;
public:
	UniformDistributionGen(Particle* model_p, Vector3 position, Vector3 velocity, double duration, int n_particle)
		:ParticleGen(model_p, position, velocity, duration, n_particle)
	{};
	std::list<Particle*> generateP() override;

};

