#include "Scene.h"
#include "Entity.h"
#include "ParticleSystem.h"
#include "ForceRegistry.h"
#include "RenderUtils.hpp"
#include <iostream>

using namespace physx;

Scene::~Scene()
{
	clean();
}

void Scene::init(physx::PxPhysics* physics, physx::PxScene* scene)
{
	_gPhysics = physics;
	_gScene = scene;
}

void Scene::clean()
{
	for (auto e : _particles) {
		delete e;
		e = nullptr;
	}
	_particles.clear();

	for (auto ps : _particleSystems) {
		delete ps;
		ps = nullptr;
	}
	_particleSystems.clear();

	for (auto r : _solids) {
		delete r;
	}
	_solids.clear();

	for (auto r : _solidSystems) {
		delete r;
	}
	_solidSystems.clear();

	delete _forceRegistry;
	_forceRegistry = nullptr;
}

void Scene::update(double t)
{
	_forceRegistry->updateForces(t);

	for (auto& e : _particles) {
		if (e && e->is_alive())e->update(t);
	}
	for (auto& e : _solids) {
		if (e && e->is_alive())e->update(t);
	}

	for (auto ps : _particleSystems) {
		if (ps)ps->update(t);
	}
	for (auto s : _solidSystems) {
		if (s)s->update(t);
	}

	for (auto it = _particles.begin(); it != _particles.end(); ) {
		if (!(*it)->is_alive()) {
			delete* it;
			*it = nullptr;
			it = _particles.erase(it);
		}
		else {
			++it;
		}
	}

	for (auto it = _solids.begin(); it != _solids.end(); ) {
		if (!(*it)->is_alive()) {
			delete* it;
			*it = nullptr;
			it = _solids.erase(it);
		}
		else {
			++it;
		}
	}
}

void Scene::addEntityWithRenderItem(Particle* p)
{
	if (p->getRenderItem() == nullptr) {
		p->create_renderItem();
	}
	_particles.push_back(p);
}

void Scene::enter()
{
	for (Entity* e : _particles) {
		if (e && !e->isRenderItemValid()) {
			e->create_renderItem();
		}
	}
	for (auto ps : _particleSystems) {
		if (ps)ps->registerAllRenderItems();
	}
	for (auto ps : _solidSystems) {
		if (ps)ps->registerAllRenderItems();
	}

	for (auto r : _solids) {
		if (r && !r->isRenderItemValid())
			r->create_renderItem();
	}
}

void Scene::exit()
{
	for (Entity* e : _particles) {
		if (e) e->deregisterRenderItem();
	}
	for (auto ps : _particleSystems) {
		if (ps)ps->deregisterAllRenderItems();
	}
	for (auto ps : _solidSystems) {
		if (ps)ps->deregisterAllRenderItems();
	}

	for (auto r : _solids) {
		if (r)r->deregisterRenderItem();
	}
}

std::string Scene::getDisplayText() const
{
	return _text;
}

void Scene::addParticleSystem(ParticleSystem* ps)
{
	if (ps != nullptr) {
		_particleSystems.push_back(ps);
	}
}

void Scene::addGlobalForce(ForceGenerator<Particle>* force)
{
	if (force) {
		for (auto& e : _particles) {
			if (e) {
				_forceRegistry->addRegistry(e, force);
			}
		}
		for (auto ps : _particleSystems) {
			if (ps)ps->addForce(force);
		}
	}
}

void Scene::removeParticle(Particle* p)
{
	for (auto it = _particles.begin(); it != _particles.end();) {
		if (*it == p && *it != nullptr) {
			delete* it;
			*it = nullptr;
			it = _particles.erase(it);
		}
	}
}

void Scene::removePacticleSystem(ParticleSystem* ps)
{
	if(!ps) return;

	for (auto it = _particleSystems.begin(); it != _particleSystems.end();) {
		if (*it == ps) {
			delete* it;
			*it = nullptr;
			it = _particleSystems.erase(it);
		}
		else {
			++it;
		}
	}
}

physx::PxMaterial* Scene::createMaterial(float staticFriction, float dynamicFriction, float restitution)
{
	return _gPhysics->createMaterial(staticFriction, dynamicFriction, restitution);
}

SolidEntity* Scene::createRigidEntity(bool dynamic, const Vector3& pos, const physx::PxGeometry& geometry, float density, physx::PxMaterial* material, const Vector4& color)
{
	SolidEntity* e = new SolidEntity(_gPhysics, _gScene, dynamic, pos, geometry, density, material, color);
	_solids.push_back(e);

	return e;

}


