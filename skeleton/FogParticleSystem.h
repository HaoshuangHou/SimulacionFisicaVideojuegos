#pragma once
#include "ParticleSystem.h"

class FogParticleSystem: public ParticleSystem {
public:
	FogParticleSystem(const Vector3& center = Vector3(0, 0, 0), float radius = 150.0f);
};