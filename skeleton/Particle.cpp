#include "Particle.h"
#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>
#include <math.h>
#include <iostream>

using namespace physx;


Particle::Particle(Vector3 const&  pos, Vector3 const& velocity, 
	Vector3 const&  acceleration,Vector4 const&  color,
	double mass,double dumping, IntegrateType t)
	
	:Entity(pos, CreateShape(PxSphereGeometry(1)), color), _vel(velocity), 
	_ac(acceleration),_intType(t), _mass(mass), _dampingValue(dumping), 
	_lifeTime(10), _iniTime(0), _color(color), _size(1),
	_force({ 0,0,0 })
{
	_ant_pos = _transform.p;
}

Particle::Particle(physx::PxShape* shape, Vector4 const& color, Vector3 const& pos, 
	Vector3 const& velocity, Vector3 const& acceleration,double mass, 
	double dumping, IntegrateType t)
	:Entity(pos, shape, color), _vel(velocity),
	_ac(acceleration), _intType(t), _mass(mass), _dampingValue(dumping),
	_lifeTime(20), _iniTime(0), _color(color), _size(1),
	_force({ 0,0,0 })
{
	_ant_pos = _transform.p;
}

Particle::Particle(const Particle& other, bool render)
	: Entity(other._transform.p, CreateShape(PxSphereGeometry(other._size)), other._color, render),
	_vel(other._vel), _ac(other._ac), _mass(other._mass), _dampingValue(other._dampingValue),
	_lifeTime(other._lifeTime), _intType(other._intType), _iniTime(0), _color(other._color),
	_ant_pos(other._transform.p), _size(other._size),
	_force({0,0,0})
{ }

Particle& Particle::operator=(const Particle& other)
{
	if (this != &other) {
		_transform.p = other._transform.p;
		_vel = other._vel;
		_ac = other._ac;
		_mass = other._mass;
		_dampingValue = other._dampingValue;
		_lifeTime = other._lifeTime;
		_intType = other._intType;
		_iniTime = 0; // Reiniciar tiempo
		_ant_pos = other._transform.p;
		_color = other._color;
	}
	return *this;
} 

void Particle::update(double t)
{
	_iniTime += t;
	if (_lifeTime > 0 && _iniTime >= _lifeTime) {
		setAlive(false);
		return;
	}

	if (!_force.isZero()) {
		_ac = (_force * (1.0 / _mass));
		std::cout << "aceleracion " << _ac.y << '\n';
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
	cleanForce();
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
		const Vector3 act_pos = _transform.p;
		_transform.p = (_transform.p * 2) - _ant_pos + _ac * pow(t, 2);
		_ant_pos = act_pos;
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

double Particle::getLifeTime() const
{
	return _lifeTime;
}

double Particle::getCLifeTime() const
{
	return _iniTime;
}

double Particle::getDamping() const
{
	return _dampingValue;
}

Vector4 Particle::getColor() const
{
	return _color;
}

void Particle::setColor(const Vector4& color)
{
	_color = color;
	if (_renderItem != nullptr) {
		_renderItem->color = color;
	}
}

void Particle::setLifeTime(double lifeTime)
{
	_lifeTime = lifeTime;
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

void Particle::setDamping(double damping)
{
	_dampingValue = damping;
}

void Particle::setTam(double tam)
{
	if (_shape) {
		const PxSphereGeometry sphere(tam);
		_shape->setGeometry(sphere);

		if (_renderItem) {
			DeregisterRenderItem(_renderItem.get());
			RegisterRenderItem(_renderItem.get());
		}
		_size = tam;
	}
}

void Particle::addForce(Vector3 const& f)
{
	_force += f;
	std::cout << "Fuerza   " << _force.y << '\n';
}

void Particle::cleanForce()
{
	_force = Vector3(0, 0, 0);
	_ac = Vector3(0, 0, 0);
}
