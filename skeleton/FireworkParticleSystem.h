#pragma once
#include "ParticleSystem.h"
#include "UniformDistributionGen.h"
#include <memory>
#include <vector>

class FireworkParticleSystem : public ParticleSystem {
private:
    Vector4 _rocketColor = Vector4(0.7f, 0.3f, 0.9f, 1.0f);
    Vector4 _explosionColor1 = Vector4(0.8f, 0.4f, 1.0f, 1.0f);
    Vector4 _explosionColor2 = Vector4(0.9f, 0.6f, 1.0f, 1.0f);
    Vector4 _explosionColor3 = Vector4(0.7f, 0.5f, 0.9f, 1.0f);
    Vector4 _sparkColor = Vector4(0.6f, 0.2f, 0.8f, 1.0f);

    Vector3 _launchPosition;
    std::unique_ptr<Particle> _rocketModel;
    std::unique_ptr<Particle> _explosionModel;
    std::unique_ptr<Particle> _sparkModel;

    UniformDistributionGen<Particle>* _explosionGen;
    UniformDistributionGen<Particle>* _sparkGen;

    void createParticleModels();
    void setupGenerators();
    void createExplosionAt(const Vector3& pos, const Vector4& color);

public:
    FireworkParticleSystem(const Vector3& launchPosition);
    ~FireworkParticleSystem();

    void update(double dt) override;
    void createFirework();
};