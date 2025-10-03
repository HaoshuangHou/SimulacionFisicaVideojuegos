#pragma once
#include "Scene.h"
class Scene1: public Scene
{
public:
	Scene1() {};

	virtual void init() override;
	virtual bool handleInput(unsigned char key) override;
	virtual void enter();
	virtual void exit();

};

