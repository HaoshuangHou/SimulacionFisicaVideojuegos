#pragma once
#include "ForceGenerator.h"
class GravityGenerator: public ForceGenerator
{
private:
    Vector3 _gravity;
public:
	GravityGenerator(const Vector3& gravity) : _gravity(gravity) {};
	virtual ~GravityGenerator() = default;

    inline virtual void updateForce(Particle* particle, double dt) override {
        if (particle != nullptr) {
            if (particle->getMass() <= 0.0f) return;
            particle->addForce(_gravity * particle->getMass());
        }
    }
};

