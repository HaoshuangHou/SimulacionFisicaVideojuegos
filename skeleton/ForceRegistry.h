#pragma once
#include <vector>
#include <algorithm>
#include "Particle.h"
#include "SolidEntity.h"
#include "ForceGenerator.h"

template <typename T>
struct ForceRegT {
    T* object;
    ForceGenerator<T>* fg;
};

class ForceRegistry {
protected:
    std::vector<ForceRegT<Particle>> particleRegistry;
    std::vector<ForceRegT<SolidEntity>> solidRegistry;

public:
    ForceRegistry() : particleRegistry(), solidRegistry() {};

    void addRegistry(Particle* particle, ForceGenerator<Particle>* fg) {
        if (particle && fg)
            particleRegistry.push_back({ particle, fg });
    }
    void addRegistry(SolidEntity* solid, ForceGenerator<SolidEntity>* fg) {
        if (solid && fg)
            solidRegistry.push_back({ solid, fg });
    }

    void remove(Particle* particle, ForceGenerator<Particle>* fg) {
        particleRegistry.erase(
            std::remove_if(particleRegistry.begin(), particleRegistry.end(),
                [&](const ForceRegT<Particle>& reg) {
                    return reg.object == particle && reg.fg == fg;
                }),
            particleRegistry.end());
    }


    void remove(SolidEntity* solid, ForceGenerator<SolidEntity>* fg) {
        solidRegistry.erase(
            std::remove_if(solidRegistry.begin(), solidRegistry.end(),
                [&](const ForceRegT<SolidEntity>& reg) {
                    return reg.object == solid && reg.fg == fg;
                }),
            solidRegistry.end());
    }

    void clearParticle(Particle* particle) {
        if (particle == nullptr) return;

        particleRegistry.erase(
            std::remove_if(particleRegistry.begin(), particleRegistry.end(),
                [&](const ForceRegT<Particle>& reg) {
                    return reg.object == particle;
                }),
            particleRegistry.end());
    }

    void clearSolid(SolidEntity* s) {
        if (s == nullptr) return;

        solidRegistry.erase(
            std::remove_if(solidRegistry.begin(), solidRegistry.end(),
                [&](const ForceRegT<SolidEntity>& reg) {
                    return reg.object == s;
                }),
            solidRegistry.end());
    }

    void clear() {
        particleRegistry.clear();
        solidRegistry.clear();
    }

    void updateForces(double dt) {
        particleRegistry.erase(
            std::remove_if(particleRegistry.begin(), particleRegistry.end(),
                [](const ForceRegT<Particle>& reg) {
                    // Remover si la partícula es nullptr o está muerta
                    return reg.object == nullptr || !reg.object->is_alive();
                }),
            particleRegistry.end());

        solidRegistry.erase(
            std::remove_if(solidRegistry.begin(), solidRegistry.end(),
                [](const ForceRegT<SolidEntity>& reg) {
                    // Remover si la partícula es nullptr o está muerta
                    return reg.object == nullptr
                        || !reg.object->is_alive()
                        || reg.object->getActor() == nullptr;
                }),
            solidRegistry.end());

        for (auto& reg : particleRegistry) {
            if (reg.object && reg.fg)
                reg.fg->updateForce(reg.object, dt);
        }

        for (auto& reg : solidRegistry) {
            if (reg.object && reg.object->is_alive()
                && reg.object->getActor()
                && reg.fg)
                reg.fg->updateForce(reg.object, dt);
        }
    }
};
