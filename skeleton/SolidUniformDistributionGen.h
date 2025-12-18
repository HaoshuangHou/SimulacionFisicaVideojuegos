#pragma once
#include "Generator.h"
#include "SolidEntity.h"

template <typename T>
class SolidUniformDistributionGen : public Generator<SolidEntity>
{
private:
	std::uniform_real_distribution<double> _uniform;
    Vector3 _des_Iner;
public:
    SolidUniformDistributionGen(SolidEntity* model_p, Vector3 position, Vector3 velocity, double duration, int n_particle)
		:Generator<SolidEntity>(model_p, position, velocity, duration, n_particle), _des_Iner()
	{};
    std::list<SolidEntity*> generate() override
    {
        std::list<SolidEntity*> list;

        void setDesInertia(const Vector3 & in) {
            _des_Iner = in;
        }

        for (int i = 0; i < n_particle; i++) {
            if (canGenerate()) {

                const Vector3 pos = _pos + Vector3(
                    _uniform(_mt) * _des_Pos.x,
                    _uniform(_mt) * _des_Pos.y,
                    _uniform(_mt) * _des_Pos.z
                );
                const Vector3 vel = _vel + Vector3(
                    _uniform(_mt) * _des_Vel.x,
                    _uniform(_mt) * _des_Vel.y,
                    _uniform(_mt) * _des_Vel.z
                );
                const double dur = _dur + _uniform(_mt) * _des_Dur;

                SolidEntity* new_solid = new SolidEntity(*_model, pos);
                const Vector3 inr = new_solid->getInertiaTensor() + Vector3(_normal(_mt) * _des_Iner.x, _normal(_mt) * _des_Iner.y, _normal(_mt) * _des_Iner.z);

                new_particle->setVelocity(vel);
                new_particle->setLifeTime(dur);
                new_solid->setInertiaTensor(inr);
                list.push_back(new_particle);
            }
        }
        return list;
    }


};

