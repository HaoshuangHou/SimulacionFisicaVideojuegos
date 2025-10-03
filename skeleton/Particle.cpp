#include "Particle.h"
#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>
#include <math.h>
#include <iostream>

using namespace physx;


Particle::Particle(Vector3 pos, Vector3 velocity, Vector3 acceleration, IntegrateType t, double mass, double dumping, double lifeTime)
	:Entity(pos, CreateShape(PxSphereGeometry(1)), { 0,1,1,1 }), _vel(velocity), _ac(acceleration), 
	_intType(t), _mass(mass), _dampingValue(dumping), _lifeTime(lifeTime), _iniTime(0)
{
	_ant_pos = _transform.p;
}

void Particle::update(double t)
{
	_iniTime += t;
	if (_lifeTime > 0 && _iniTime >= _lifeTime) {
		setAlive(false);
		return;
	}

	switch (_intType)
	{
	case EULER:
		integrate_euler(t);
		break;
	case EULER_SEMI_IMPLICITO:
		integrate_euler_semi_implicito(t);
		break;
	case VERLET:
		integrate_verlet(t);
		break;
	default:
		break;
	}

}

void Particle::integrate_euler(double t)
{
	_transform.p = _transform.p + t * _vel;
	_vel = _vel + t * _ac;
	_vel = _vel * pow(_dampingValue, t);
}

void Particle::integrate_euler_semi_implicito(double t)
{
	_vel = _vel + t * _ac;
	_vel = _vel * pow(_dampingValue, t);
	_transform.p = _transform.p + t * _vel;
}

void Particle::integrate_verlet(double t)
{
	if (_ant_pos == _transform.p) integrate_euler_semi_implicito(t);
	else {
		Vector3 act_pos = _transform.p;
		_transform.p = (_transform.p * 2) - _ant_pos + _ac * pow(t, 2);
		_ant_pos = act_pos;
		//std::cout << "aceleracion: " << _ac.y << "  delta time: " << t << "\n";
	}
}

Vector3 Particle::getVelocity() const
{
	return _vel;
}

Vector3 Particle::getAcceleration() const
{
	return _ac;
}

double Particle::getMass() const
{
	return _mass;
}

void Particle::setVelocity(const Vector3& velocity)
{
	_vel = velocity;
}

void Particle::setAcceleration(const Vector3& acceleration)
{
	_ac = acceleration;
}

void Particle::setMass(double mass)
{
	_mass = mass;
}

