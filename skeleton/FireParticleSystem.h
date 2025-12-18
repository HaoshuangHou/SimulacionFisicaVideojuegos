#pragma once
#include "ParticleSystem.h"
#include "NormalDistributionGen.h"

class FireParticleSystem: public ParticleSystem {
public:
	FireParticleSystem(const Vector3& center = Vector3(0, 0, 0), float radius = 10.0f,
		const Vector4& start = Vector4(1.0f, 0.2f, 0.0f, 1.0f), 
		const Vector4& end = Vector4(1.0f, 1.0f, 0.3f, 1.0f),
		float systemLifeTime = -1.0f);
	virtual void update(double dt) override;

private:
	void updateParticleColor(Particle* particle);
	float _elapsedTime = 0.0f;
	float _systemLifeTime;
	NormalDistributionGen<Particle>* fireGen = nullptr;
};