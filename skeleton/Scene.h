#pragma once
#include <vector>
#include <string>
#include <list>
#include <PxPhysicsAPI.h>
#include "ForceRegistry.h"
#include "RenderUtils.hpp"
#include "Projectil.h"
#include "ParticleSystem.h"
#include "SolidSystem.h"
#include "Particle.h"
#include "SolidEntity.h"

#include "GravityGenerator.h"
#include "WindGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionGenerator.h"
class Scene
{
public:
	Scene()
		: _particles(), _text(), _particleSystems(), _forceRegistry(new ForceRegistry())
	{};
	virtual ~Scene();

	Scene(const Scene& s) = delete;  
	Scene& operator=(const Scene& s) = delete;

	virtual void init(physx::PxPhysics* physics, physx::PxScene* scene);
	virtual void clean();
	virtual void handleInput(unsigned char key) = 0;
	virtual void handleSpecialInput(int key) {};
	virtual void update(double t);
	virtual void addEntityWithRenderItem(Particle* p);
	virtual void enter();
	virtual void exit();
	std::string getDisplayText() const;

	void addParticleSystem(ParticleSystem* ps);
	void addGlobalForce(ForceGenerator<Particle>* force);
	void removeParticle(Particle* p);
	void removePacticleSystem(ParticleSystem* ps);

protected:
	physx::PxPhysics* _gPhysics = nullptr;
	physx::PxScene* _gScene = nullptr;

	std::vector<Particle*> _particles;
	std::list<ParticleSystem*> _particleSystems;
	std::list<SolidSystem*> _solidSystems;

	std::vector<SolidEntity*> _solids;

	std::string _text;
	ForceRegistry* _forceRegistry;


	physx::PxMaterial* createMaterial(float staticFriction, float dynamicFriction, float restitution);

	SolidEntity* createRigidEntity(
		bool dynamic,
		const Vector3& pos,
		const physx::PxGeometry& geometry,
		float density = 1.0f,
		physx::PxMaterial* material = nullptr,
		const Vector4& color = Vector4(1, 1, 1, 1)
	);

};

