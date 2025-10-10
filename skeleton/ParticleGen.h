#pragma once
#include <random>
#include <list>
#include "core.hpp"
#include "Particle.h"

class ParticleGen
{
private:
	std::mt19937 _mt;
	Vector3 _pos;
	Vector3 _vel;
	double _dur;
	double probGen;
	int n_particle;
public:
	ParticleGen() : _pos(0, 0, 0), _vel(1, 0, 0), _dur(1), n_particle(1), _mt() {};
	ParticleGen(Vector3 position, Vector3 velocity, double duration, int n_particle)
		:_pos(position), _vel(velocity), _dur(duration), n_particle(n_particle), _mt() {};

	virtual std::list<Particle*> generateP() = 0;

	//Getters y Setters
	inline Vector3 getPos() const { return _pos; };
	inline void setPos(const Vector3& pos) { _pos = pos; };

	inline Vector3 getVel() const { return _vel; };
	inline void setVel(const Vector3& vel) { _vel = vel; };

	inline double getDuration() const { return _dur; };
	inline void  setDuration(double duration) { _dur = duration; };

};

