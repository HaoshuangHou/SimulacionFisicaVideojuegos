#include "Scene2.h"
#include <iostream>

void Scene2::init()
{
	_camera = GetCamera();
	_text = "ESCENA 2: PROYECTILES, SIGUIENTE ESCENA(Q)";
}

void Scene2::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
	case '1':
	{
		create_projectil(CANON, 10);
		break;
	}
	case '2':
	{
		create_projectil(TANQUE, 50);
		break;
	}
	case '3':
	{
		create_projectil(PISTOLA, 100);
		break;
	}
	case '4':
	{
		create_projectil(PISTOLA, 250);
		break;
	}
	default:
		break;
	}
}

void Scene2::create_projectil(ProjectilType t, float sim_speed)
{
	Vector3 dir=_camera->getDir();
	dir.normalize();
	const Vector3 ini_pos = _camera->getTransform().p;
	Projectil* p = new Projectil(ini_pos, sim_speed * dir, t);
	addEntityWithRenderItem(p);
}
