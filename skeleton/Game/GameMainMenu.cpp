#include "GameMainMenu.h"
#include "../BubbleSystem.h"
#include "../AlgaeParticleSystem.h"

void GameMainMenu::init(physx::PxPhysics* physics, physx::PxScene* scene)
{
	_gPhysics = physics;
	_gScene = scene;
	_text = "";

	title = "BubbleFish";
	BubbleSystem* b = new BubbleSystem({ 0,10,0 }, physics, scene);
	_solidSystems.push_back(b);


	physx::PxMaterial* material = _gPhysics->createMaterial(0.8f, 0.7f, 0.1f);
}

void GameMainMenu::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
	default:
		sceneManager->changeScene(6);
		break;
	}
}

void GameMainMenu::render_interface()
{
	const int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	const int screenHeight = glutGet(GLUT_WINDOW_HEIGHT);

	float navyBlue[4] = { 0.0f, 0.0f, 0.5f, 1.0f };
	float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	drawText(title, titlePos.x, titlePos.y, 80, true, true, false, navyBlue);

	drawText("Pulsa una tecla para iniciar", screenWidth / 2.0f, screenHeight * 0.5f, 35, true, true, false, white);
}
void GameMainMenu::repositionObjects()
{
	titlePos = getRelativePosition(0.5, 0.6);
}
Vector3 GameMainMenu::getRelativePosition(float relX, float relY, float z) const
{
	const int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	const int screenHeight = glutGet(GLUT_WINDOW_HEIGHT);

	const float screenX = relX * screenWidth;
	const float screenY = relY * screenHeight;

	return Vector3(screenX, screenY, z);
}