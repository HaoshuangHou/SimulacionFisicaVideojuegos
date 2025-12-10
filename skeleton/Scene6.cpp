#include "Scene6.h"
#include "BubbleSystem.h"

using namespace physx;

void Scene6::init(physx::PxPhysics* physics, physx::PxScene* scene)
{
	_gPhysics = physics;
	_gScene = scene;

	_text = "ESCENA 6: SOLIDOS, SIGUIENTE ESCENA(Q)";


	createRigidEntity(
		false,                      // estático
		Vector3(0, -1, 0),           // posición
		PxBoxGeometry(200, 1, 200),   // suelo
		0,                          // densidad (no importa si no es dinámico)
		nullptr,                    // usa material por defecto
		Vector4(0.5, 0.5, 0.5, 1)   // color
	);

	BubbleSystem* b = new BubbleSystem(Vector3(0,10,0),_gPhysics, _gScene);
	_solidSystems.push_back(b);
}

void Scene6::enter()
{
	Scene::enter();
}

void Scene6::exit()
{
	Scene::exit();
}

void Scene6::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
		case '1':
		{
			break;
		}
		default: {
			break;
		}
	}
}