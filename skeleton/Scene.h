#pragma once
#include <vector>

class Entity;
class Scene
{
public:
	Scene() = default;
	virtual ~Scene();

	Scene(const Scene& s) = delete;  
	Scene& operator=(const Scene& s) = delete;

	void addEntity(Entity* e);
	virtual void init() = 0;
	virtual void clean();
	virtual bool handleInput(unsigned char key) = 0;
	virtual void update(double t);
	virtual void enter();
	virtual void exit();

protected:
	std::vector<Entity*> _entities;
};

