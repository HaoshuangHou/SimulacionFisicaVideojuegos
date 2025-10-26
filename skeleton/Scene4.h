#pragma once
#include "Scene.h"
#include "RenderUtils.hpp"
#include "Projectil.h"

class ParticleSystem;
class Scene4: public Scene
{
public:
	Scene4() {};
	virtual void init() override;
	virtual void handleInput(unsigned char key) override;
};

