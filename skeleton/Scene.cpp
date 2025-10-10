#include "Scene.h"
#include "Entity.h"
#include "RenderUtils.hpp"

Scene::~Scene()
{
	clean();
}

void Scene::clean()
{
	for (auto e : _entities) delete e;
	_entities.clear();
}

void Scene::update(double t)
{
	for (auto& e : _entities) {
		e->update(t);
	}

	for (auto it = _entities.begin(); it != _entities.end(); ) {
		if (!(*it)->is_alive()) {
			DeregisterRenderItem((*it)->getRenderItem());
			delete* it;
			it = _entities.erase(it);
		}
		else {
			++it;
		}
	}
}

void Scene::enter()
{
	for (auto e : _entities) {
		RegisterRenderItem(e->getRenderItem());
	}
}

void Scene::exit()
{
	for (auto e : _entities) {
		DeregisterRenderItem(e->getRenderItem());
	}
	clean();
}


