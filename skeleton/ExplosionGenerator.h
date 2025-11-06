#pragma once
#include "ForceGenerator.h"
#include <PxPhysicsAPI.h>
#include "core.hpp"
#include "RenderUtils.hpp"

class ExplosionGenerator: public ForceGenerator
{
protected:
    double _K;               
    double _t;           

    double _expan_vel;
    double _elapsedTime;
    bool _active;

public:
    ExplosionGenerator(Vector3 const& pos, double radius, double K, double t, double expan_vel = 2.0)
        :_K(K), _t(t), _elapsedTime(0), _active(false), _expan_vel(expan_vel)
    {
        _center = pos;
        _radius = radius;
        createRenderItem(CreateShape(physx::PxSphereGeometry(radius)), _center, Vector4(1, 0, 0, 0.5));
        setVisible(false);
    };
	virtual ~ExplosionGenerator() = default;

    void active(bool a) {
        _active = a;
        if (a) {
            _elapsedTime = 0;
            _radius = 1.0;
        }
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

        _radius = _expan_vel * _elapsedTime;
        if (r < _radius) {
            double T = exp(-_elapsedTime / _t);
            double forceMagnitude = (_K / (r * r)) * T;
            Vector3 forceDirection = diff.getNormalized();
            Vector3 force = forceDirection * forceMagnitude;

            particle->addForce(force);
        }
    }
};

