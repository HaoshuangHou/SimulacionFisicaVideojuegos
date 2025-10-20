#include "NormalDistributionGen.h"

std::list<Particle*> NormalDistributionGen::generateP()
{
    std::list<Particle*> list;
    
    for (int i = 0; i < n_particle; i++) {
        if (canGenerateParticle()) {

            Particle* new_particle = new Particle(*_model);

            Vector3 pos = _pos + _normal(_mt) * _des_Pos;
            Vector3 vel = _vel + _normal(_mt) * _des_Vel;
            double dur = _dur + _normal(_mt) * _des_Dur;

            new_particle->setPosition(pos);
            new_particle->setVelocity(vel);
            new_particle->setLifeTime(dur);

            list.push_back(new_particle);
        }
    } 
    return list;
}
