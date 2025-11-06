#pragma once
#include <vector>
#include <algorithm>
#include "Particle.h"
#include "ForceGenerator.h"

class ForceRegistry {
protected:
    struct ForceReg {
        Particle* particle;
        ForceGenerator* fg;
    };

    std::vector<ForceReg> registry;

public:
    void addRegistry(Particle* particle, ForceGenerator* fg) {
        if (particle && fg)
            registry.push_back({ particle, fg });
    }

    void remove(Particle* particle, ForceGenerator* fg) {
        registry.erase(
            std::remove_if(registry.begin(), registry.end(),
                [&](const ForceReg& reg) {
                    return reg.particle == particle && reg.fg == fg;
                }),
            registry.end());
    }

    void clearParticle(Particle* particle) {
        if (particle == nullptr) return;

        registry.erase(
            std::remove_if(registry.begin(), registry.end(),
                [&](const ForceReg& reg) {
                    return reg.particle == particle;
                }),
            registry.end());
    }

    void clear() {
        registry.clear();
    }

    void updateForces(double dt) {
        registry.erase(
            std::remove_if(registry.begin(), registry.end(),
                [](const ForceReg& reg) {
                    // Remover si la partícula es nullptr o está muerta
                    return reg.particle == nullptr || !reg.particle->is_alive();
                }),
            registry.end());

        for (auto& reg : registry) {
            if (reg.particle && reg.fg) {
                reg.fg->updateForce(reg.particle, dt);
            }
        }
    }
};
