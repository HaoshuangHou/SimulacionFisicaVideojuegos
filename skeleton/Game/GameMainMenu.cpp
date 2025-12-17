#include "GameMainMenu.h"
#include "../BubbleSystem.h"

void GameMainMenu::init(physx::PxPhysics* physics, physx::PxScene* scene)
{
	_gPhysics = physics;
	_gScene = scene;
	_text = "";

	title = "JUEGO";
	BubbleSystem* b = new BubbleSystem({ 0,0,0 }, physics, scene);
	_solidSystems.push_back(b);
}

void GameMainMenu::handleInput(unsigned char key)
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

void GameMainMenu::render_interface()
{
	float navyBlue[4] = { 0.0f, 0.0f, 0.5f, 1.0f };

	drawText(title, titlePos.x, titlePos.y, 80, true, true, false, navyBlue);

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