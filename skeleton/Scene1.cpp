#include "Scene1.h"
#include "Particle.h"

void Scene1::init()
{
	_text = "ESCENA 1: PARTICULAS , PASAR DE ESCENA CON Q";
}

void Scene1::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
	case '1':
	{
		Particle* p = new Particle(Vector3(0, 10, 0), Vector3(5, 10, 0),
			Vector3(0, -9.8, 0), EULER, 1.0, 0.9, 1.0);
		_entities.push_back(p);
		break;
	}
	case '2':
	{
		Particle* p = new Particle(Vector3(0, 10, 0), Vector3(5, 10, 0),
			Vector3(0, -9.8, 0), EULER_SEMI_IMPLICITO);
		_entities.push_back(p);
		break;
	}
	case '3':
	{
		Particle* p = new Particle(Vector3(0, 10, 0), Vector3(5, 10, 0),
			Vector3(0, -9.8, 0), VERLET);
		_entities.push_back(p);
		break;
	}
	default:
		break;
	}
}
