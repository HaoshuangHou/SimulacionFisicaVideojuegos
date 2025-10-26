#include "FogParticleSystem.h"
#include "Particle.h"
#include "UniformDistributionGen.h"

FogParticleSystem::FogParticleSystem(const Vector3& center, float radius)
    : ParticleSystem(center, radius)
{
    create_model_particle(0.2);

    _startColor = Vector4(0.7f, 0.7f, 0.75f, 1.0f);

    if (_model_particle) {
        _model_particle->setPosition(_center);
        _model_particle->setAcceleration(Vector3(0.0f, 0.05f, 0.0f));
        _model_particle->setColor(_startColor);
    }

    //configurar el generador
    UniformDistributionGen* fogGen = new UniformDistributionGen(
        _model_particle,
        _center,
        Vector3(0.01f, 0.01f, 0.01f),
        6.0f,                       
        5                         
    );

    //las desviaciones
    fogGen->setDesPos(Vector3(_radius * 0.5f, _radius * 0.5f, _radius * 0.5f));
    fogGen->setDesVel(Vector3(0.1f, 0.1f, 0.1f));
    fogGen->setDesDur(4.0f);

    addGenerator(fogGen);
}