#include "Scene6.h"
#include "BubbleSystem.h"

using namespace physx;

void Scene6::init(physx::PxPhysics* physics, physx::PxScene* scene)
{
	_gPhysics = physics;
	_gScene = scene;

	_text = "ESCENA 6: SOLIDOS, SIGUIENTE ESCENA(Q)";


	createRigidEntity(
		false,                      // estatico
		Vector3(0, -1, 0),			//posicion
		PxBoxGeometry(200, 1, 200), //geometria
		0,                         
		nullptr,                
		Vector4(0.5, 0.5, 0.5, 1) 
	);

	//SolidEntity* a = createRigidEntity(
	//	true,                      // dinamico
	//	Vector3(0, 100, 0),			//posicion
	//	PxBoxGeometry(5.0f, 5.0f, 5.0f), //geometria
	//	1.0f,
	//	nullptr,
	//	Vector4(1, 0.5, 0.5, 1)
	//);
	//GravityGenerator<SolidEntity>* g = new GravityGenerator<SolidEntity>(Vector3(0, -9.8, 0));
	//_forceRegistry->addRegistry(a, g);

	BubbleSystem* b = new BubbleSystem(Vector3(-30,30,-30),_gPhysics, _gScene);
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