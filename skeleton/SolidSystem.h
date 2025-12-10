#pragma once
#include <list>
#include <memory>
#include "core.hpp"

class SolidEntity;
class Scene;

class SolidSystem {
protected:
	Vector3 _center;
	float _radius;
	Vector4 _startColor;
	Vector4 _endColor;

	std::list<std::unique_ptr<SolidEntity>> _solids;

	SolidEntity* _model_solid;
	Scene* _scene;

	void create_model_solid();
	void create_model_solid(float tam);

	virtual void delete_solid(); 

	bool isSolidOutsideArea(SolidEntity* solid);

public:
	SolidSystem(Scene* scene, const physx::PxVec3& center, float r);
	virtual ~SolidSystem();

	virtual void update(double dt);

	void registerAllRenderItems();
	void deregisterAllRenderItems();
};