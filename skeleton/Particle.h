#pragma once
#include <PxPhysicsAPI.h>
#include "core.hpp"
#include "Entity.h"

enum IntegrateType {
	EULER,
	EULER_SEMI_IMPLICITO,
	VERLET
};

class RenderItem;
class Particle: public Entity
{
public:
	Particle(Vector3 pos, Vector3 velocity, Vector3 acceleration, IntegrateType t = EULER, double mass = 1, double dumping = 0.98, double lifeTime = 10.0);

	Particle(const Particle&) = delete;
	Particle& operator=(const Particle&) = delete;

	void update(double t) override;

	Vector3 getVelocity() const;
	Vector3 getAcceleration() const;
	double getMass()const;

	void setVelocity(const Vector3& velocity);
	void setAcceleration(const Vector3& acceleration);
	void setMass(double mass);

protected:
	Vector3 _vel;
	Vector3 _ac;
	double _mass;
	double _lifeTime;
	double _dampingValue;

private:
	IntegrateType _intType;
	double _iniTime;
	Vector3 _ant_pos; //posicion anterior

	void integrate_euler(const double t);
	void integrate_euler_semi_implicito(double t);
	void integrate_verlet(double t);
};

