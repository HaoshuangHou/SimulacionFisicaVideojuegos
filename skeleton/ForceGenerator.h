#pragma once
#include "core.hpp"
#include "Particle.h"

class ForceGenerator
{
public:
	ForceGenerator() = default;
	virtual ~ForceGenerator() = default;
	virtual void updateForce(Particle* particle, double dt) = 0;
};

