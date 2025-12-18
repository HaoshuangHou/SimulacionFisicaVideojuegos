#pragma once
#include "ParticleSystem.h"


class AlgaeParticleSystem : public ParticleSystem {
public:
	AlgaeParticleSystem(const Vector3& center = Vector3(0, 0, 0), float radius = 10.0f);

	void update(double dt) override;
private:
	void updateParticleColor(Particle* particle);
};