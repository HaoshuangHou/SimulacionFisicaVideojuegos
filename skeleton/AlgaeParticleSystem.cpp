#include "AlgaeParticleSystem.h"
#include "Particle.h"
#include "WindGenerator.h"
#include "WhirlwindGenerator.h"
#include "UniformDistributionGen.h"

#include <cmath>

AlgaeParticleSystem::AlgaeParticleSystem(const Vector3& center, float radius)
    : ParticleSystem(center, radius)
{
    _startColor = Vector4(0.0f, 0.4f, 0.0f, 1.0f);
    _endColor = Vector4(0.6f, 1.0f, 0.2f, 1.0f);

    _model_particle = new Particle(CUBE, Vector4(0.2f, 0.6f, 0.2f, 1.0f), Vector3(0, 0, 0));
    _model_particle->setTam(0.4f);
    _model_particle->setLifeTime(1.0f);

    _center = center;

    if (_model_particle) {
        _model_particle->setPosition(_center);
        _model_particle->setDamping(0.85f);                    
        _model_particle->setLifeTime(3.0f);                     
        _model_particle->setColor(_startColor);
    }

    UniformDistributionGen<Particle>* algaeGen = new UniformDistributionGen<Particle>(
        _model_particle, 
        _center, 
        Vector3(0, 1.0f, 0), 
        2.0f, 
        1);

    algaeGen->setSpawnProbability(0.05);
    algaeGen->setDesPos(Vector3(0.0f, 0.1f, 0.0f));
    algaeGen->setDesVel(Vector3(0.3f, 0.1f, 0.3f));
    algaeGen->setDesDur(0.5f);

    addGenerator(algaeGen);

    addForce(new WindGenerator<Particle>(
        Vector3(0, 0, 0),
        50.f,
        Vector3(0.0f, 1.0f, 0.0f),
        0.2f
    ));

    addForce(new WhirlwindGenerator<Particle>(
        Vector3(0, 0, 0),
        50.f,
        0.4,
        0.1 
    ));
}

void AlgaeParticleSystem::update(double dt)
{
    ParticleSystem::update(dt);
    for (auto& p : _particles) {
        updateParticleColor(p.get());
    }
}
void AlgaeParticleSystem::updateParticleColor(Particle* particle)
{
    if (!particle) return;
    float t = particle->getCLifeTime() / particle->getLifeTime();
    Vector4 color = _startColor * (1.0f - t) + _endColor * t;

    particle->setColor(color);
}