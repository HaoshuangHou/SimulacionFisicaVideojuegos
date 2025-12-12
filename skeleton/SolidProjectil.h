#pragma once
#include "SolidEntity.h"
class SolidProjectil: public SolidEntity
{
public:
    SolidProjectil(Vector3 pos, Vector3 sim_velocity, Vector4 const& color = Vector4(1, 1, 1, 1));
    virtual void update(double t) override;
private:
    double _real_mass;
    double _real_speed;

    void configurarDesdeTipo(Vector3 sim_velocity);
};

