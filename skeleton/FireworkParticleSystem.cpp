#include "FireworkParticleSystem.h"
#include "FireworkParticleSystem.h"
#include "Particle.h"
#include "ForceRegistry.h"
#include <cstdlib>
#include <ctime>

FireworkParticleSystem::FireworkParticleSystem(const Vector3& launchPosition)
    : ParticleSystem(launchPosition, 1.0f), _launchPosition(launchPosition)
{
    std::srand(std::time(nullptr));
    createParticleModels();
    setupGenerators();
}

FireworkParticleSystem::~FireworkParticleSystem()
{
    delete _explosionGen;
    delete _sparkGen;
}

void FireworkParticleSystem::createParticleModels()
{
    // Modelo para cohetes
    _rocketModel = std::make_unique<Particle>();
    _rocketModel->setVelocity({ 0,25,0 });
    _rocketModel->setPosition(_launchPosition);
    _rocketModel->setLifeTime(0.8f);
    _rocketModel->setColor(_rocketColor);
    _rocketModel->setTam(1.0f);

    // Modelo para partículas de explosión
    _explosionModel = std::make_unique<Particle>();
    _explosionModel->setVelocity({ 20,20,20 });
    _explosionModel->setLifeTime(0.6f);
    _explosionModel->setColor(_explosionColor1);
    _explosionModel->setTam(0.5f);

    // Modelo para chispas
    _sparkModel = std::make_unique<Particle>();
    _sparkModel->setVelocity({ 5,10,5 });
    _sparkModel->setLifeTime(0.3f);
    _sparkModel->setColor(_sparkColor);
    _sparkModel->setMass(0.2f);
    _sparkModel->setTam(0.3f);
}

void FireworkParticleSystem::setupGenerators()
{
    _explosionGen = new UniformDistributionGen(
        _explosionModel.get(),
        Vector3(0, 0, 0),                   
        Vector3(0, 0, 0),                   
        0.6f,                               
        6                                 
    );
    _explosionGen->setDesPos(Vector3(0.5f, 0.5f, 0.5f));
    _explosionGen->setDesVel(Vector3(6.0f, 6.0f, 6.0f));
    _explosionGen->setDesDur(0.2f);   

    _sparkGen = new UniformDistributionGen(
        _sparkModel.get(),
        Vector3(0, 0, 0),                   
        Vector3(0, 0, 0),                   
        0.3f,                               
        3                                
    );
    _sparkGen->setDesPos(Vector3(0.5f, 0.5f, 0.5f)); 
    _sparkGen->setDesVel(Vector3(3.0f, 3.0f, 3.0f));
    _sparkGen->setDesDur(0.2f);
}

void FireworkParticleSystem::createExplosionAt(const Vector3& pos, const Vector4& color)
{
    _explosionGen->setColor(color);
    _explosionGen->setPos(pos);

    auto explosionParticles = _explosionGen->generateP();
    for (auto& explosionParticle : explosionParticles) {
        _particles.push_back(std::unique_ptr<Particle>(explosionParticle));
        for (auto& force : _forces) {
            _forceRegistry->addRegistry(explosionParticle, force.get());
        }
    }
}

void FireworkParticleSystem::createFirework()
{
    Particle* rocket = new Particle(*_rocketModel.get());
    _particles.push_back(std::unique_ptr<Particle>(rocket));
}

void FireworkParticleSystem::update(double dt)
{

    _forceRegistry->updateForces(dt);

    std::vector<Particle*> deadRockets;
    std::vector<Particle*> deadExplosion1;
    std::vector<Particle*> deadExplosion2;
    std::vector<Particle*> deadExplosion3;

    for (auto& p : _particles) {
        p->update(dt);

        if (!p->is_alive()) {
            const Vector4 color = p->getColor();

            if (color == _rocketColor) {
                deadRockets.push_back(p.get());
            }
            else if (color == _explosionColor1) {
                deadExplosion1.push_back(p.get());
            }
            else if (color == _explosionColor2) {
                deadExplosion2.push_back(p.get());
            }
            else if (color == _explosionColor3) {
                deadExplosion3.push_back(p.get());
            }
        }
    }

    for (auto* rocket : deadRockets) {
        createExplosionAt(rocket->getPos(), _explosionColor1);
    }

    for (auto* explosion : deadExplosion1) {
        createExplosionAt(explosion->getPos(), _explosionColor2);
    }

    for (auto* explosion : deadExplosion2) {
        createExplosionAt(explosion->getPos(), _explosionColor3);
    }

    for (auto* explosionParticle : deadExplosion3) {
        _sparkGen->setPos(explosionParticle->getPos());
        auto sparks = _sparkGen->generateP();
        for (auto& spark : sparks) {
            _particles.push_back(std::unique_ptr<Particle>(spark));
            for (auto& force : _forces) {
               _forceRegistry->addRegistry(spark, force.get());
            }
        }
    }

    _particles.remove_if([this](std::unique_ptr<Particle>& p) {
        if (!p->is_alive()) {
            _forceRegistry->clearParticle(p.get());
            return true;
        }
        return false;
        });
}