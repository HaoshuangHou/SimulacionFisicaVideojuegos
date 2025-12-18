#include "ForceParticleSystem.h"
#include "UniformDistributionGen.h"

ForceParticleSystem::ForceParticleSystem(ForceGenerator<Particle>* forceGenerator, const Vector4& color, bool isWhirlwind)
	: ParticleSystem(forceGenerator->getCenter(), forceGenerator->getRadius()),
	_forceGenerator(forceGenerator), _particleColor(color), _isWhirlwind(isWhirlwind)
{
    create_model_particle(0.15f);

    if (_model_particle) {
        _model_particle->setPosition(_center);
        _model_particle->setColor(_particleColor);
        _model_particle->setLifeTime(_isWhirlwind ? 5.0f : 4.0f);
        _model_particle->setMass(1.0f);
    }

    UniformDistributionGen<Particle>* forceGen = new UniformDistributionGen<Particle>(
        _model_particle,
        _center,
        Vector3(0.3f, 0.2f, 0.3f),
        _isWhirlwind ? 5.0f : 4.0f,
        _isWhirlwind ? 4 : 8
    );

    if (_isWhirlwind) {
        forceGen->setDesPos(Vector3(_radius * 0.6f, _radius * 0.4f, _radius * 0.6f));
        forceGen->setDesVel(Vector3(1.2f, 0.8f, 0.3f));
        forceGen->setDesDur(2.5f);
    }
    else {
        forceGen->setDesPos(Vector3(_radius * 0.6f, _radius * 0.5f, _radius * 0.6f));
        forceGen->setDesVel(Vector3(0.5f, 0.5f, 0.5f));
        forceGen->setDesDur(2.0f);
    }
    addGenerator(forceGen);
    addForce(forceGenerator);
}

void ForceParticleSystem::setVisible(bool visible)
{
    const float alpha = visible ? 1.0f : 0.0f;

    size_t nullCount = 0;
    for (auto& p : _particles) {
        if (p)p->setColor(Vector4(_particleColor.x, _particleColor.y, _particleColor.z, alpha));
    }

    if (_model_particle) {
        _model_particle->setColor(Vector4(_particleColor.x, _particleColor.y, _particleColor.z, alpha));
    }
}
