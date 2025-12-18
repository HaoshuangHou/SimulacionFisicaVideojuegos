#include "Scene.h"
#include "Entity.h"
#include "ParticleSystem.h"
#include "ForceRegistry.h"
#include "RenderUtils.hpp"
#include <iostream>
#include "SceneManager.h"
using namespace physx;

SceneManager* Scene::sceneManager = nullptr;

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
		r = nullptr;
	}
	_solids.clear();

	for (auto r : _solidSystems) {
		delete r;
		r = nullptr;
	}
	_solidSystems.clear();

	delete _forceRegistry;
	_forceRegistry = nullptr;
}

void Scene::update(double t)
{
	updateViewportFromScreen();

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
		SolidEntity* solid = *it;

		if (!solid->is_alive()) {
			// 1. Limpiar del registro de fuerzas (IMPORTANTE)
			if (_forceRegistry) {
				_forceRegistry->clearSolid(solid);
			}

			// 2. Eliminar físicamente de PhysX
			if (PxRigidActor* actor = solid->getActor()) {
				if (PxScene* scene = actor->getScene()) {
					scene->removeActor(*actor);
				}
				actor->release();
			}
			solid->deregisterRenderItem();

			// 4. Liberar memoria
			delete solid;

			// 5. Eliminar del vector
			it = _solids.erase(it);
		}
		else {
			++it;
		}
	}
	_forceRegistry->updateForces(t);
}

void Scene::addEntityWithRenderItem(Particle* p)
{
	if (p->getRenderItem() == nullptr) {
		p->create_renderItem();
	}
	_particles.push_back(p);
}

void Scene::addEntityWithRenderItem(SolidEntity* s)
{
	if (s->getRenderItem() == nullptr) {
		s->create_physicsObject();
		s->create_renderItem();
	}
	_solids.push_back(s);
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
		if (r) {
			r->create_physicsObject();
			if (!r->isRenderItemValid()) {
				r->create_renderItem();
			}
		}
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
		if (!r) continue;
		if (PxRigidActor* actor = r->getActor()) {
			if (PxScene* scene = actor->getScene())
				scene->removeActor(*actor);
			actor->release();
		}
		r->deregisterRenderItem();
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

void Scene::removeSolid(SolidEntity* s)
{
	if (!s) return;

	auto it = std::find(_solids.begin(), _solids.end(), s);
	if (it == _solids.end()) return;

	physx::PxActor* actor = s->getActor();

	if (actor && _gScene) {
		_gScene->removeActor(*actor);
		actor->release();   
	}

	s->deregisterRenderItem();
	delete s;

	_solids.erase(it);
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

void Scene::removeSolidSystem(SolidSystem* ss)
{
	if (!ss) return;

	for (auto it = _solidSystems.begin(); it != _solidSystems.end();) {
		if (*it == ss) {
			(*it)->killAll();
			/*ss->deregisterAllRenderItems();
			delete* it;
			*it = nullptr;
			it = _solidSystems.erase(it);*/
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

void Scene::setupCamera()
{
	if (GetCamera()) {
		GetCamera()->setEye({ 0.1f, 0.2f, 25.0f });
		GetCamera()->setDir({ 0.0f, 0.0f, -1.0f });
	}
}

Vector3 Scene::getRelativePosition(float relX, float relY, float z) const {
	float worldX = relX * _worldWidth - _worldWidth / 2.0f;
	float worldY = relY * _worldHeight - _worldHeight / 2.0f;
	return Vector3(worldX, worldY, z);
}
void Scene::updateViewportFromScreen() {
	int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	int	screenHeight = glutGet(GLUT_WINDOW_HEIGHT);
	float screenAspect = (float)screenWidth / (float)screenHeight;

	float baseWorldHeight = 24;
	float baseWorldWidth = 44;
	float baseAspect = baseWorldWidth / baseWorldHeight;

	float oldWorldWidth = _worldWidth;
	float oldWorldHeight = _worldHeight;

	// Calcular nuevas dimensiones
	if (screenAspect > baseAspect) {
		_worldHeight = baseWorldHeight;
		_worldWidth = baseWorldHeight * screenAspect;
	}
	else {
		_worldWidth = baseWorldWidth;
		_worldHeight = baseWorldWidth / screenAspect;
	}

	// Si las dimensiones cambiaron, reposicionar objetos
	if (oldWorldWidth != _worldWidth || oldWorldHeight != _worldHeight) {
		repositionObjects();
	}
}
