#pragma once
#include "ParticleSystem.h"
#include "NormalDistributionGen.h"
#include <unordered_set>

class FireworkParticleSystem : public ParticleSystem {
private:
    int _maxGenerations;
    double _timeSinceLastFirework;
    double _fireworkInterval;
    NormalDistributionGen* _rocketGen;
    NormalDistributionGen* _explosionGen;
    NormalDistributionGen* _finalGen;

    std::unordered_set<Particle*> _rockets;
    std::unordered_set<Particle*> _explosions;
    std::unordered_set<Particle*> _final;

public:
    FireworkParticleSystem(const Vector3& center=Vector3(0,0,0), float radius=20.0f, int maxGenerations = 3);
    virtual ~FireworkParticleSystem();
    virtual void update(double dt) override;

private:
    void createNewFirework();
    void createFinalFlash(const Vector3& position, const Vector4& color);
    void processFireworkDeaths();
    Vector4 getRandomColor();
};