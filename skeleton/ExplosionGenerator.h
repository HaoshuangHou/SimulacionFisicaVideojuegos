#pragma once
#include "ForceGenerator.h"
#include <PxPhysicsAPI.h>
#include "core.hpp"
#include "RenderUtils.hpp"

class ExplosionGenerator: public ForceGenerator
{
protected:
    Vector3 _center;

    double _R;
    double _K;               
    double _t;           

    double _elapsedTime;
    bool _active;

public:
    ExplosionGenerator(Vector3 const& pos, double radius, double K, double t)
        : _center(pos), _R(radius) , _K(K), _t(t), _elapsedTime(0), _active(false)
    {
        RegisterRenderItem(
            new RenderItem(CreateShape(physx::PxSphereGeometry(_R)),
                new physx::PxTransform(_center),
                Vector4(0, 1, 1, 0.5))
        );
    };
	virtual ~ExplosionGenerator() = default;

    void activate() {
        _active = true;
        _elapsedTime = 0;
    }

    void deactivate() {
        _active = false;
    }

    inline virtual void updateForce(Particle* particle, double dt) override {
        if (particle == nullptr) return;

        if (_active) {
            _elapsedTime += dt;

            if (_elapsedTime > 4 * _t) {
                _active = false;
                return;
            }
        }
        else return;

        Vector3 pos = particle->getPos();
        Vector3 diff = pos - _center;
        double r = diff.magnitude();

        if (r < _R) {
            double T = exp(-_elapsedTime / _t);
            double forceMagnitude = (_K / (r * r)) * T;
            Vector3 forceDirection = diff.getNormalized();
            Vector3 force = forceDirection * forceMagnitude;

            particle->addForce(force);
        }
    }
};

