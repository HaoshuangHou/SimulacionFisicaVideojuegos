#pragma once
#include "../Scene.h"
class GameMainMenu : public Scene
{
public:
	GameMainMenu() {};

	virtual void init(physx::PxPhysics* physics, physx::PxScene* scene) override;
	virtual void handleInput(unsigned char key) override;
	virtual void render_interface() override;
private:
	virtual	void repositionObjects() override;
	virtual Vector3 getRelativePosition(float relX, float relY, float z = 0.0f) const;
	std::string title;
	Vector3 titlePos;
};

