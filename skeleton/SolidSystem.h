#pragma once
#include <list>
#include <memory>
#include <PxPhysicsAPI.h>
#include "core.hpp"
#include "Generator.h"
#include "ForceGenerator.h"

class SolidEntity;
class Scene;
class ForceRegistry;

class SolidSystem {
protected:
	Vector3 _center;
	float _radius;
	Vector4 _startColor;
	Vector4 _endColor;

	physx::PxPhysics* _physics;
	physx::PxScene* _scene;


	std::list<std::unique_ptr<SolidEntity>> _solids;
	std::list<Generator<SolidEntity>*> _generators;
	std::list<std::unique_ptr<ForceGenerator<SolidEntity>>> _forces;
	std::unique_ptr<ForceRegistry> _forceRegistry;

	SolidEntity* _model_solid;

	void create_model_solid();

	virtual void delete_solid(SolidEntity* s);

	bool isSolidOutsideArea(SolidEntity* solid);
	void addGenerator(Generator<SolidEntity>* gen);
	void addForce(ForceGenerator<SolidEntity>* f);

public:
	SolidSystem(physx::PxPhysics* physics, physx::PxScene* scene, const physx::PxVec3& center, float r);
	virtual ~SolidSystem();

	virtual void update(double dt);

	void registerAllRenderItems();
	void deregisterAllRenderItems();
};