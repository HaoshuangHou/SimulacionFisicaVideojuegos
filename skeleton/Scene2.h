#pragma once
#include "Scene.h"

class Scene2: public Scene
{
private:
	Camera* _camera;
public:
	Scene2() {};
	virtual void init() override;
	virtual void handleInput(unsigned char key) override;
	void create_projectil(ProjectilType t, float sim_speed);
};

