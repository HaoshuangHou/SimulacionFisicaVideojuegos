#pragma once
#include <vector>
#include <string>
#include <list>

class Entity;
class ParticleSystem;
class Scene
{
public:
	Scene() : _entities(), _text(), _particleSystems() {};
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
};

