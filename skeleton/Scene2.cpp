#include "Scene2.h"
#include <iostream>

void Scene2::init()
{
	_camera = GetCamera();
}

void Scene2::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
	case '1':
	{
		create_projectil(CANON, 5);
		break;
	}
	case '2':
	{
		Projectil* p = new Projectil(Vector3(0, 10, 0), Vector3(5, 0, 0), CANON);
		_entities.push_back(p);
		std::cout << "Aceleracion Canon: " << p->getAcceleration().y << "\n";
		break;
	}
	case '3':
	{
		Projectil* p = new Projectil(Vector3(0, 10, 0), Vector3(5, 0, 0), TANQUE);
		_entities.push_back(p);
		std::cout << "Aceleracion tanque: " << p->getAcceleration().y << "\n";
		break;
	}
	case '4':
	{
		Projectil* p = new Projectil(Vector3(0, 10, 0), Vector3(10, 0, 0), PISTOLA);
		_entities.push_back(p);
		std::cout << "Aceleracion pistola: " << p->getAcceleration().y << "\n";
		break;
	}
	case '5':
	{
		Projectil* p = new Projectil(Vector3(10, 10, 0), Vector3(250, 0, 0), LASER);
		_entities.push_back(p);
		std::cout << "Aceleracion laser: " << p->getAcceleration().y << "\n";
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
	_entities.push_back(p);
}
