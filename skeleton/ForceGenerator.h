#pragma once
#include "core.hpp"

class Particle;
class ForceGenerator
{
public:
	ForceGenerator() : _force() {};
	virtual ~ForceGenerator();
	virtual void calculateForce(Particle* p) = 0;

private:
	Vector3 _force;
};

