#include "UniformDistributionGen.h"

std::list<Particle*> UniformDistributionGen::generateP()
{
    std::list<Particle*> list;
    
    for (int i = 0; i < n_particle; i++) {
        if (canGenerateParticle()) {

            Particle* new_particle = new Particle(*_model);
            Vector3 pos = _pos + Vector3(
                (_uniform(_mt) * 2.0 - 1.0) * _des_Pos.x,
                (_uniform(_mt) * 2.0 - 1.0) * _des_Pos.y,
                (_uniform(_mt) * 2.0 - 1.0) * _des_Pos.z
            );
            Vector3 vel = _vel + Vector3(
                (_uniform(_mt) * 2.0 - 1.0) * _des_Vel.x,
                (_uniform(_mt) * 2.0 - 1.0) * _des_Vel.y,
                (_uniform(_mt) * 2.0 - 1.0) * _des_Vel.z
            );
            double dur = _dur + _uniform(_mt) * _des_Dur;

            new_particle->setPosition(pos);
            new_particle->setVelocity(vel);
            new_particle->setLifeTime(dur);

            list.push_back(new_particle);
        }
    } 
    return list;
}
