#pragma once
#include "Scene.h"

class Scene3: public Scene
{
public:
	Scene3() {};
	virtual void init() override;
	virtual void handleInput(unsigned char key) override;
	void createParticleSystem();
};

