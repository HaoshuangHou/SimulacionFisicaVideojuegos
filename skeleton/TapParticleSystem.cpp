#include "TapParticleSystem.h"
#include "Particle.h"
#include "UniformDistributionGen.h"
#include "GravityGenerator.h"

TapParticleSystem::TapParticleSystem(const Vector3& center, const Vector3& dir, float radius)
    : ParticleSystem(center, radius)
{
    create_model_particle();

    _startColor = Vector4(0.6f, 0.8f, 1.0f, 1.0f);

    if (_model_particle) {
        _model_particle->setPosition(_center);
        _model_particle->setVelocity(dir * 2);
        _model_particle->setColor(_startColor);
        _model_particle->setDamping(0.99f);
    }

    //configurar el generador
    UniformDistributionGen<Particle>* waterGen = new UniformDistributionGen<Particle>(
        _model_particle,
        _center,
        Vector3(0.01f, 0.01f, 0.01f),
        6.0f,                       
        5                         
    );

    //las desviaciones
    waterGen->setDesPos(Vector3(1.0f, 0.02f, 1.0f));
    waterGen->setDesVel(Vector3(0.3f, 0.1f, 0.3f));
    waterGen->setDesDur(1.5f);

    addGenerator(waterGen);  
}