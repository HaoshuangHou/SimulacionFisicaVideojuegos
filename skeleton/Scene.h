#pragma once
#include <vector>
#include <string>
#include <list>
#include "ForceRegistry.h"
#include "RenderUtils.hpp"
#include "Projectil.h"
#include "ParticleSystem.h"
#include "Entity.h"

class Scene
{
public:
	Scene() : _entities(), _text(), _particleSystems(), _forceRegistry(new ForceRegistry()) {};
	virtual ~Scene();

	Scene(const Scene& s) = delete;  
	Scene& operator=(const Scene& s) = delete;

	virtual void init() = 0;
	virtual void clean();
	virtual void handleInput(unsigned char key) = 0;
	virtual void update(double t);
	virtual void addEntityWithRenderItem(Entity* e);
	virtual void enter();
	virtual void exit();
	std::string getDisplayText() const;

	void addParticleSystem(ParticleSystem* ps);

protected:
	std::vector<Entity*> _entities;
	std::list<ParticleSystem*> _particleSystems;
	std::string _text;
	ForceRegistry* _forceRegistry;
};

