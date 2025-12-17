#include "GameOverMenu.h"

void GameOverMenu::init(physx::PxPhysics* physics, physx::PxScene* scene)
{
	_gPhysics = physics;
	_gScene = scene;
	_text = "";

}

void GameOverMenu::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
	case '1':
	{
	}
	default:
		break;
	}
}

void GameOverMenu::render_interface()
{
}
void GameOverMenu::enter()
{
	current_projection_mode = PROJ_ORTHOGRAPHIC;
	Scene::enter();
	setupCamera();
}

void GameOverMenu::exit()
{
	current_projection_mode = PROJ_PERSPECTIVE;
	Scene::exit();
	if (GetCamera()) GetCamera()->resetCamera();
}