#include "Scene6.h"

using namespace physx;

void Scene6::init(physx::PxPhysics* physics, physx::PxScene* scene, physx::PxMaterial* material)
{
	_gPhysics = physics;
	_gScene = scene;
	_gMaterial = material;

	_text = "ESCENA 6: SOLIDOS, SIGUIENTE ESCENA(Q)";

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