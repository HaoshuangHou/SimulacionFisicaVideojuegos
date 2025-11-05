#include "Scene.h"
#include "Entity.h"
#include "ParticleSystem.h"
#include "ForceRegistry.h"
#include "RenderUtils.hpp"

Scene::~Scene()
{
	clean();
}

void Scene::clean()
{
	for (auto e : _particles) delete e;
	_particles.clear();

	for (auto ps : _particleSystems) delete ps;
	_particleSystems.clear();

	delete _forceRegistry;
}

void Scene::update(double t)
{
	_forceRegistry->updateForces(t);

	for (auto& e : _particles) {
		if (e && e->is_alive())e->update(t);
	}

	for (auto ps : _particleSystems) {
		if (ps)ps->update(t);
	}

	for (auto it = _particles.begin(); it != _particles.end(); ) {
		if (!(*it)->is_alive()) {
			delete* it;
			it = _particles.erase(it);
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

}

void Scene::exit()
{
	for (Entity* e : _particles) {
		if (e) e->deregisterRenderItem();
	}
	for (auto ps : _particleSystems) {
		if (ps)ps->deregisterAllRenderItems();
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

void Scene::addGlobalForce(ForceGenerator* force)
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


