#pragma once
#include "ParticleSystem.h"

class TapParticleSystem: public ParticleSystem {
public:
	TapParticleSystem(const Vector3& center = Vector3(0, 0, 0), const Vector3& dir = Vector3(0, -1, 0), float radius = 20.0f);
	virtual ~TapParticleSystem() = default;
};