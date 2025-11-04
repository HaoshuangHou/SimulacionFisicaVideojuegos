#pragma once
#include "WindGenerator.h"

class WhirlwindGenerator : public WindGenerator
{
protected:
    double _K; 

public:
    WhirlwindGenerator(Vector3 const& pos, double radius, double K, double k1, double k2 = 0)
        : WindGenerator(pos, radius, Vector3(0, 0, 0), k1, k2), _K(K)
    {
    };

    virtual ~WhirlwindGenerator() = default;

    inline virtual void updateForce(Particle* particle, double dt) override {
        if (particle == nullptr) return;

        const Vector3 r = particle->getPos() - _center;
        _wind_velocity= _K * Vector3(-r.y, r.x, 0);

        WindGenerator::updateForce(particle, dt);
    }
};