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

	/*for (auto it = _entities.begin(); it != _entities.end(); ) {
		if (!(*it)->is_alive()) {
			DeregisterRenderItem((*it)->getRenderItem());
			delete* it;
			it = _entities.erase(it);
		}
		else {
			++it;
		}
	}*/
}

void Scene::addEntityWithRenderItem(Entity* e)
{
	if (e->getRenderItem() == nullptr) {
		e->create_renderItem();
	}
	_entities.push_back(e);
}

void Scene::enter()
{
	//for (Entity* e : _entities) {
	//	e->create_renderItem();
	//}
}

void Scene::exit()
{
	for (Entity* e : _entities) {
		//if(e->getRenderItem()!=nullptr)DeregisterRenderItem(e->getRenderItem());
	}
}

std::string Scene::getDisplayText() const
{
	return _text;
}


