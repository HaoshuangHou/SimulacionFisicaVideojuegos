#include "Scene1.h"
#include "Particle.h"

void Scene1::init()
{
	_text = "ESCENA 1: PARTICULAS, SIGUIENTE ESCENA(Q)";
	_gravityGenerator = new GravityGenerator(Vector3(0, -9.8, 0));
}

void Scene1::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
	case '1':
	{
		Particle* p = new Particle(Vector3(0, 10, 0), Vector3(5, 10, 0),
			Vector3(0, -9.8, 0), Vector4(1, 1, 1, 1), EULER);
		addEntityWithRenderItem(p);
		_forceRegistry->addRegistry(p, _gravityGenerator);
		break;
	}
	case '2':
	{
		Particle* p = new Particle(Vector3(0, 10, 0), Vector3(5, 10, 0),
			Vector3(0, -9.8, 0), Vector4(1, 0, 1, 1), EULER_SEMI_IMPLICITO);
		addEntityWithRenderItem(p);
		_forceRegistry->addRegistry(p, _gravityGenerator);
		break;
	}
	case '3':
	{
		Particle* p = new Particle(Vector3(0, 10, 0), Vector3(5, 10, 0),
			Vector3(0, -9.8, 0), Vector4(1, 1, 0, 1), VERLET);
		addEntityWithRenderItem(p);
		_forceRegistry->addRegistry(p, _gravityGenerator);
		break;
	}
	default:
		break;
	}
}
