#pragma once
#include "Scene.h"
class Scene1: public Scene
{
private:
	GravityGenerator* _gravityGenerator;
public:
	Scene1() {};

	virtual void init() override;
	virtual void handleInput(unsigned char key) override;

};

