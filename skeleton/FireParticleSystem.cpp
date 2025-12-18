#include "FireParticleSystem.h"
#include "Particle.h"
#include <cmath>

FireParticleSystem::FireParticleSystem(const Vector3& center, float radius,
    const Vector4& start, const Vector4& end, float systemLifeTime)
    : ParticleSystem(center, radius), _systemLifeTime(systemLifeTime)
{
    create_model_particle(radius / 10);
    _center = center;

    _startColor = start;
    _endColor = end;

    if (_model_particle) {
        _model_particle->setPosition(_center);
        _model_particle->setDamping(0.9f);                    
        _model_particle->setLifeTime(2.5f);                     
        _model_particle->setColor(_startColor);
    }

    //(Particle* model_p, Vector3 position, Vector3 velocity, double duration, int n_particle
    fireGen = new NormalDistributionGen<Particle>(
        _model_particle, 
        _center, 
        Vector3(0, 3.0f, 0), 
        1.5f, 
        1);

    // Configurar variaciones para efecto de fuego
    fireGen->setDesPos(Vector3(0.8f, 0.2f, 0.8f));
    fireGen->setDesVel(Vector3(1.2f, 3.0f, 1.2f));
    fireGen->setDesDur(0.8f);             

    addGenerator(fireGen);
}

void FireParticleSystem::update(double dt)
{
    _elapsedTime += dt;
    if (_systemLifeTime > 0.0f &&_elapsedTime >= _systemLifeTime)
    {
        fireGen->setSpawnProbability(0.0);
    }


    ParticleSystem::update(dt);
    for (auto& p : _particles) {
        updateParticleColor(p.get());
    }
}

void FireParticleSystem::updateParticleColor(Particle* particle)
{
    if (!particle) return;

    float t = particle->getCLifeTime() / particle->getLifeTime();
    Vector4 color = _startColor * (1.0f - t) + _endColor * t;

    particle->setColor(color);
}