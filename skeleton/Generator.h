#pragma once
#include <random>
#include <list>
#include <iostream>
#include "core.hpp"

template<typename T>
class Generator
{
protected:
	std::mt19937 _mt;
	Vector3 _pos;
	Vector3 _vel;
	double _dur;
	int n_particle;
	T* _model;

	Vector3 _des_Pos;
	Vector3 _des_Vel;
	double _des_Dur;
	double _des_Mass;

	double _spawnProbability = 0.5;
public:
	Generator() : _pos(0, 0, 0), _vel(1, 0, 0), _dur(1), n_particle(1),
		_mt(std::random_device{}()), _model(nullptr), _des_Pos(0, 0, 0),_des_Dur(0),_des_Vel(0, 0, 0), _des_Mass(0) {};

	Generator(T* model_p, Vector3 position, Vector3 velocity, double duration, int n_particle)
		:_pos(position), _vel(velocity), _dur(duration), n_particle(n_particle),
		_mt(std::random_device{}()), _model(model_p), _des_Pos(), _des_Dur(), _des_Vel(), _des_Mass() {};

	virtual ~Generator() = default;

	virtual std::list<T*> generate() = 0;

	//Getters y Setters
	inline Vector3 getPos() const { return _pos; };
	inline void setPos(const Vector3& pos) { _pos = pos; };

	inline void setColor(const Vector4& color) { _model->setColor(color); };

	inline Vector3 getVel() const { return _vel; };
	inline void setVel(const Vector3& vel) { _vel = vel; };

	inline double getDuration() const { return _dur; };
	inline void  setDuration(double duration) { _dur = duration; };

	inline Vector3 getDesPos() const { return _des_Pos ; };
	inline void  setDesPos(Vector3 desPos) { _des_Pos = desPos; };

	inline Vector3 getDesVel () const { return _des_Vel; };
	inline void  setDesVel(Vector3 desVel) { _des_Vel = desVel; };

	inline double getDesDur() const { return _des_Dur; };
	inline void  setDesDur(double desDur) { _des_Dur = desDur; };

	inline void  setDesMass(double desMass) { _des_Mass = desMass; };

	inline void setSpawnProbability(double p) {
		_spawnProbability = max(0.0,min(p, 1.0));
	}
	inline bool canGenerate() {
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		return dist(_mt) < _spawnProbability;
	}

};

