#pragma once
#include "ParticleGen.h"

class Particle;
class NormalDistributionGen : public ParticleGen
{
private:
	std::normal_distribution<double> _normal;
public:
	NormalDistributionGen(Particle* model_p, Vector3 position, Vector3 velocity, double duration, int n_particle)
		:ParticleGen(model_p, position, velocity, duration, n_particle)
	{};
	std::list<Particle*> generateP() override;

};

