#pragma once
#include "ForceGenerator.h"
#include <iostream>
template <typename T>
class GravityGenerator: public ForceGenerator<T>
{
private:
    Vector3 _gravity;
public:
	GravityGenerator(const Vector3& gravity) : _gravity(gravity) {};
	virtual ~GravityGenerator() = default;

    inline virtual void updateForce(T* particle, double dt) override {
        if (particle != nullptr) {
            if (particle->getMass() <= 0.0f) return;
            Vector3 force = _gravity * particle->getMass();
            particle->addForce(force);
        }
    }
};

