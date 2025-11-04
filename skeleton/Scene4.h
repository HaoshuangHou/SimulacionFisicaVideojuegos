#pragma once
#include "Scene.h"

class ExplosionGenerator;
class GravityGenerator;
class Scene4: public Scene
{
protected:
	ExplosionGenerator* _explosionGenerator = nullptr;
	GravityGenerator* _gravityGenerator = nullptr;
public:
	Scene4() {};
	virtual void init() override;
	virtual void handleInput(unsigned char key) override;
};

