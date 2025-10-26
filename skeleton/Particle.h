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
	Particle(Vector3 const& pos = Vector3(0, 0, 0), Vector3 const& velocity = Vector3(0, 0, 0),
		Vector3 const& acceleration = Vector3(0, 0, 0), Vector4 const& color = Vector4(1, 1, 1, 1) ,
		IntegrateType t = EULER_SEMI_IMPLICITO,double mass = 1, double dumping = 0.98, 
		double lifeTime = 10.0);

	Particle(const Particle& other);
	Particle& operator=(const Particle& other);

	void update(double t) override;

	Vector3 getVelocity() const;
	Vector3 getAcceleration() const;
	double getMass()const;
	double getLifeTime() const;
	double getCLifeTime() const;
	double getDamping() const;
	

	void setColor(const Vector4& color);
	void setLifeTime(double lifeTime);
	void setVelocity(const Vector3& velocity);
	void setAcceleration(const Vector3& acceleration);
	void setMass(double mass);
	void setPosition(const Vector3& pos);
	void setDamping(double damping);
	void setTam(double tam);
	void addForce(Vector3 const& f);
	void cleanForce();

protected:
	Vector3 _vel;
	Vector3 _ac;
	Vector3 _force;
	double _mass;
	double _lifeTime;
	double _dampingValue;
	Vector4 _color;
	double _size;

private:
	IntegrateType _intType;
	double _iniTime;
	Vector3 _ant_pos; //posicion anterior

	void integrate_euler(const double t);
	void integrate_euler_semi_implicito(double t);
	void integrate_verlet(double t);
};

