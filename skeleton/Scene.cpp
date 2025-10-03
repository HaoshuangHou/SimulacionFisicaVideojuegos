#include "Scene.h"
#include "Entity.h"
#include "RenderUtils.hpp"

Scene::~Scene()
{
	clean();
}

void Scene::addEntity(Entity* e)
{
	_entities.push_back(e);
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

	//_entities.erase(
	//	std::remove_if(_entities.begin(), _entities.end(),
	//		[](const Entity*& e) {
	//			return !e->is_alive();
	//		}),
	//	_entities.end()
	//);
}

void Scene::enter()
{
	for (auto e : _entities) {
		//RegisterRenderItem(e->getRenderItem());
	}
}

void Scene::exit()
{
	for (auto e : _entities) {
		//DeregisterRenderItem(e->getRenderItem());
	}
}


