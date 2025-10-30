#pragma once
#include "Scene.h"

class Scene4: public Scene
{
public:
	Scene4() {};
	virtual void init() override;
	virtual void handleInput(unsigned char key) override;
};

