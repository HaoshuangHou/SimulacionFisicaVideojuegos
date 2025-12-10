#pragma once
#include "WindGenerator.h"
template <typename T>
class WhirlwindGenerator : public WindGenerator<T>
{
protected:
    double _K; 
    bool _is2d;
public:
    WhirlwindGenerator(Vector3 const& pos, double radius, double K, double k1, double k2 = 0, bool is2D = false)
        : WindGenerator(pos, radius, Vector3(0, 0, 0), k1, k2), _K(K), _is2d(is2D)
    {
        createRenderItem(CreateShape(physx::PxSphereGeometry(_radius)), _center, Vector4(1, 1, 0, 0.5));
        setVisible(false);
    };

    virtual ~WhirlwindGenerator() = default;

    inline virtual void updateForce(T* particle, double dt) override {
        if (particle == nullptr) return;

        const Vector3 r = particle->getPos() - _center;
        if (_is2d) {
            _wind_velocity = _K * Vector3(-r.y, r.x + 5.0, 0);
        }
        else {
            _wind_velocity = _K * Vector3(-r.z, 20 - r.y, r.x);
        }

        WindGenerator::updateForce(particle, dt);
    }
};