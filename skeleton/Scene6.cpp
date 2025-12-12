#include "Scene6.h"
#include "BubbleSystem.h"

using namespace physx;

void Scene6::init(physx::PxPhysics* physics, physx::PxScene* scene)
{
	_gPhysics = physics;
	_gScene = scene;

	_text = "ESCENA 6: SOLIDOS, SIGUIENTE ESCENA(Q)";


	//suelo
	createRigidEntity(
		false,                      // estatico
		Vector3(0, -1, 0),			//posicion
		PxBoxGeometry(200, 1, 200), //geometria
		0,                         
		nullptr,                
		Vector4(0.5, 0.5, 0.5, 1) 
	);

	//cubo dinamico
	physx::PxMaterial* cubo1Material = physics->createMaterial(0.1f, 0.5f, 2.0f);
	physx::PxMaterial* cubo2Material = physics->createMaterial(0.1f, 0.2f, 1.0f);
	SolidEntity* a1 = createRigidEntity(
		true,                      // dinamico
		Vector3(20, 10, 0),			//posicion
		PxBoxGeometry(5.0f, 5.0f, 5.0f), //geometria
		1.0f,
		cubo1Material,
		Vector4(1, 0.5, 0.5, 1)
	);
	SolidEntity* a2 = createRigidEntity(
		true,                      // dinamico
		Vector3(-10, 10, 0),			//posicion
		PxSphereGeometry(5.0f), //geometria
		1.0f,
		cubo2Material,
		Vector4(0.5, 1, 0.5, 1)
	);
	GravityGenerator<SolidEntity>* g = new GravityGenerator<SolidEntity>(Vector3(0, -9.8, 0));
	_forceRegistry->addRegistry(a1, g);
	_forceRegistry->addRegistry(a2, g);

	//sistema de burbuja
	BubbleSystem* b = new BubbleSystem(Vector3(-30,20,-30),_gPhysics, _gScene);
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

}