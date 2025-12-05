#pragma once
#include "ForceGenerator.h"

class ElasticBandForceGenerator : public ForceGenerator
{
private:
    double _k;
    double _resting_length;
    Particle* _other;
public:
    ElasticBandForceGenerator(double k, double resting_length, Particle* other)
        : _k(k), _resting_length(resting_length), _other(other) {};
	virtual ~ElasticBandForceGenerator() = default;

    inline void set_k(double k) {_k = k; }

    inline virtual void updateForce(Particle* particle, double dt) override {
        
        Vector3 relative_pos_vector = _other->getPos() - particle->getPos();
        Vector3 force;
        const float length = relative_pos_vector.normalize();
        if (length <= _resting_length)
            return;
        const float delta_x = length - _resting_length;
        force = relative_pos_vector * delta_x * _k;

        particle->addForce(force);
    }
};

