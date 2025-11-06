#pragma once
#include "ParticleSystem.h"
#include "WindGenerator.h"
#include "WhirlwindGenerator.h"

class ForceParticleSystem : public ParticleSystem {
private:
    ForceGenerator* _forceGenerator;
    Vector4 _particleColor;
    bool _isWhirlwind;
    bool _active;

public:
    ForceParticleSystem(ForceGenerator* forceGenerator, const Vector4& color, bool isWhirlwind = false);
    void setVisible(bool visible);
};

