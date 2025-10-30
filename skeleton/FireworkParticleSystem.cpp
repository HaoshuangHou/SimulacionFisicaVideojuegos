#include "FireworkParticleSystem.h"
#include "Particle.h"

FireworkParticleSystem::FireworkParticleSystem(const Vector3& center, float radius, int maxGenerations)
    : ParticleSystem(center, radius),
    _maxGenerations(maxGenerations),
    _timeSinceLastFirework(0.0),
    _fireworkInterval(2.0f)
{
    create_model_particle(); 

    if (_model_particle) {
        _model_particle->setPosition(_center);
        _model_particle->setVelocity(Vector3(0, 5.0f, 0));
        _model_particle->setColor(Vector4(1, 1, 1, 1));
        _model_particle->setLifeTime(1.0f);
    }

    _rocketGen = new NormalDistributionGen(
        _model_particle,
        _center,
        Vector3(0, 12.0f, 0),      
        2.5f,                     
        1              
    );
    _rocketGen->setDesPos(Vector3(1.0f, 0.0f, 1.0f));    
    _rocketGen->setDesVel(Vector3(0.5f, 3.0f, 0.5f));   
    _rocketGen->setDesDur(1.0f);                       


    Particle* explosionModel = new Particle(*_model_particle, false);
    explosionModel->setTam(0.7f);
    explosionModel->setAcceleration(Vector3(0, -9.0f, 0));

    _explosionGen = new NormalDistributionGen(
        explosionModel,
        Vector3(0, 0, 0),          // Posición relativa (se ajustará)
        Vector3(0, 0, 0),          // Velocidad base 0 (la variación hace el trabajo)
        1.8f,                      // Duración
        10                         // 10 partículas por explosión
    );
    _explosionGen->setDesPos(Vector3(0.1f, 0.1f, 0.1f)); // Poca variación posición
    _explosionGen->setDesVel(Vector3(8.0f, 8.0f, 8.0f)); // Alta variación para explosión radial
    _explosionGen->setDesDur(1.0f);                      // Variación duración

    Particle* sparkleModel = new Particle(*_model_particle, false);
    sparkleModel->setTam(0.4f);
    sparkleModel->setAcceleration(Vector3(0, -5.0f, 0));

    _finalGen = new NormalDistributionGen(
        sparkleModel,
        Vector3(0, 0, 0),
        Vector3(0, 1.0f, 0),       // Pequeño impulso hacia arriba
        1.3f,                      // Duración
        5                          // 5 chispas
    );
    _finalGen->setDesPos(Vector3(0.5f, 0.2f, 0.5f));   // Variación posición
    _finalGen->setDesVel(Vector3(4.0f, 3.0f, 4.0f));   // Variación velocidad
    _finalGen->setDesDur(1.0f);                        // Variación duración

}

FireworkParticleSystem::~FireworkParticleSystem()
{
    delete _rocketGen;
    delete _explosionGen;
    delete _finalGen;

    _rockets.clear();
    _explosions.clear();
    _final.clear();
}

void FireworkParticleSystem::update(double dt)
{
    ParticleSystem::update(dt);

    // Generar nuevos fuegos artificiales
    _timeSinceLastFirework += dt;
    if (_timeSinceLastFirework >= _fireworkInterval) {
        createNewFirework();
        _timeSinceLastFirework = 0.0;
    }

    processFireworkDeaths();
}

void FireworkParticleSystem::createNewFirework()
{
    auto rockets = _rocketGen->generateP();

    for (Particle* rocket : rockets) {
        rocket->setColor(getRandomColor());
        _rockets.insert(rocket);
    }

    //_particles.splice(_particles.end(), rockets);

}

void FireworkParticleSystem::processFireworkDeaths()
{
    std::vector<Particle*> deadRockets;
    for (Particle* r : _rockets) {
        if (!r->is_alive()) {
            deadRockets.push_back(r);
            Vector3 pos = r->getTransform().p;
            Vector4 color = r->getColor();

            _explosionGen->setPos(pos);
            auto explosionParticles = _explosionGen->generateP();

            for (Particle* p : explosionParticles) {
                p->setColor(color);
                _explosions.insert(p);
            }
           // _particles.splice(_particles.end(), explosionParticles);
        }
    }

    std::vector<Particle*> deadExplosions;
    for (Particle* e : _explosions) {
        if (!e->is_alive()) {
            deadExplosions.push_back(e);
            Vector3 pos = e->getTransform().p;
            Vector4 color = e->getColor();

            _finalGen->setPos(pos);
            auto sparkleParticles = _finalGen->generateP();

            // Aplicar color a las chispas
            for (Particle* p : sparkleParticles) {
                p->setColor(color);
                _final.insert(p);
            }
          //  _particles.splice(_particles.end(), sparkleParticles);
        }
    }
    for (Particle* dead : deadRockets) {
        _rockets.erase(dead);
    }
    for (Particle* dead : deadExplosions) {
        _explosions.erase(dead);
    }
}
Vector4 FireworkParticleSystem::getRandomColor()
{
    static std::vector<Vector4> colors = {
        Vector4(1.0f, 0.2f, 0.2f, 1.0f),  // Rojo
        Vector4(0.2f, 1.0f, 0.2f, 1.0f),  // Verde  
        Vector4(0.2f, 0.4f, 1.0f, 1.0f),  // Azul
        Vector4(1.0f, 1.0f, 0.2f, 1.0f),  // Amarillo
        Vector4(1.0f, 0.2f, 1.0f, 1.0f),  // Magenta
        Vector4(0.2f, 1.0f, 1.0f, 1.0f)   // Cian
    };

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, colors.size() - 1);

    return colors[dist(gen)];
}