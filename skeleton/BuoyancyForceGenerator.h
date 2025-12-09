#pragma once
#include "ForceGenerator.h"
#include <iostream>

class BuoyancyForceGenerator : public ForceGenerator
{
private:
    double _height;
    double _volume;
    double _liquid_density;
    double _gravity = 9.8;
    double h0;

public:
    BuoyancyForceGenerator(double h, double volume, double density, double surface_height)
        : _height(h), _volume(volume), _liquid_density(density), h0(surface_height) {};
	virtual ~BuoyancyForceGenerator() = default;

    inline virtual void updateForce(Particle* particle, double dt) override {
        
        const double h = particle->getPos().y;
        double immersed = 0.0;
        if (h - h0 > _height * 0.5) {
            immersed = 0.0;
        }
        else if (h0 - h > _height * 0.5) {
            immersed = 1.0;
        }
        else {
            immersed = (h0 - h) / _height + 0.5;
        }

        Vector3 force(0, 0, 0);
        force.y = _liquid_density * _volume * immersed * _gravity;

        particle->addForce(force);
    }
};

