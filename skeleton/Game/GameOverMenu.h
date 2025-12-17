#pragma once
#include "../Scene.h"
class GameOverMenu : public Scene
{
public:
	GameOverMenu() {};

	virtual void init(physx::PxPhysics* physics, physx::PxScene* scene) override;
	virtual void handleInput(unsigned char key) override;
	virtual void render_interface() override;
	virtual void enter() override;
	virtual void exit() override;
};

