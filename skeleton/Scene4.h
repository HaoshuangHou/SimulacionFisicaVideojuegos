#pragma once
#include "Scene.h"

class ExplosionGenerator;
class GravityGenerator;
class WindGenerator;
class WhirlwindGenerator;
class Scene4: public Scene
{
protected:
	ExplosionGenerator* _explosionGenerator = nullptr;
	GravityGenerator* _gravityGenerator = nullptr;
	WindGenerator* _windGenerator = nullptr;
	WhirlwindGenerator* _whirlwindGenerator = nullptr;
public:
	Scene4() {};
	virtual void init() override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void handleInput(unsigned char key) override;
};

