#pragma once
#include "Particle.h"

class Projectil: public Particle
{
public:
	Projectil(Vector3 pos, Vector3 real_velocity, double real_mass);
};

