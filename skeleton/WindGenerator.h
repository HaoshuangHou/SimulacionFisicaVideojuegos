#pragma once
#include "ForceGenerator.h"
#include <PxPhysicsAPI.h>
#include "core.hpp"
#include "RenderUtils.hpp"

template <typename T>
class WindGenerator: public ForceGenerator<T>
{
protected:
    Vector3 _wind_velocity;
    double _k1;
    double _k2;
public:
    WindGenerator(Vector3 const& pos, double radius, const Vector3& wind_vel, double k1, double k2 = 0)
        :_wind_velocity(wind_vel), _k1(k1), _k2(k2)
    {
        _center = pos;
        _radius = radius;
        createRenderItem(CreateShape(physx::PxSphereGeometry(_radius)), _center,Vector4(0, 1, 0, 0.1));
        setVisible(false);
    };
	virtual ~WindGenerator() = default;

    inline virtual void updateForce(T* particle, double dt) override {
        if (particle == nullptr) return;

        if ((particle->getPos() - _center).magnitude() > _radius) return;

        const Vector3 relVel = _wind_velocity - particle->getVelocity();
        const float relSpeed = relVel.magnitude();

        const Vector3  force = _k1 * relVel + _k2 * relSpeed * relVel;

        particle->addForce(force);
    }
};

