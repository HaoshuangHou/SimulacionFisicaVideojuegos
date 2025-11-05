#pragma once
#include "Scene.h"

class FireworkParticleSystem;
class Scene3: public Scene
{
private:
	FireworkParticleSystem* f;
public:
	Scene3() {};
	virtual void init() override;
	virtual void handleInput(unsigned char key) override;
	void createParticleSystem();

};

