#pragma once
#include "Generator.h"

template <typename T>
class UniformDistributionGen : public Generator<T>
{
private:
	std::uniform_real_distribution<double> _uniform;
public:
	UniformDistributionGen(T* model_p, Vector3 position, Vector3 velocity, double duration, int n_particle)
		:Generator<T>(model_p, position, velocity, duration, n_particle)
	{};
    std::list<T*> generate() override
    {
        std::list<T*> list;

        for (int i = 0; i < n_particle; i++) {
            if (canGenerate()) {

                T* new_particle = new T(*_model);
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


};

