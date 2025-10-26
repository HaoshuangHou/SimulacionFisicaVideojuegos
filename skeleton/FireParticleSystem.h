#pragma once
#include "ParticleSystem.h"


class FireParticleSystem: public ParticleSystem {
public:
	FireParticleSystem(const Vector3& center = Vector3(0, 0, 0), float radius = 10.0f);
	virtual ~FireParticleSystem() = default;
	virtual void update(double dt) override;

private:
	void updateParticleColor(Particle* particle);
};