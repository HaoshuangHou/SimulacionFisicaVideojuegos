#include "Scene4.h"
#include "GravityGenerator.h"


void Scene4::init()
{
	_text = "ESCENA 4: PARTICULAS CON FUERZAS, PASAR DE ESCENA CON Q";

	GravityGenerator* gravity = new GravityGenerator(Vector3(0, -9.8, 0));
	Particle* p1 = new Particle(
		Vector3(-5, 10, 0), Vector3(0, 0, 0),
		Vector3(0, 0, 0), Vector4(1, 0, 0, 1)  // Rojo
	);
	p1->setMass(1);
	_forceRegistry->addRegistry(p1, gravity);
	_entities.push_back(p1);

	Particle* p3 = new Particle(
		Vector3(-5, 10, 0), Vector3(0, 0, 0),
		Vector3(0, 0, 0), Vector4(1, 1, 0, 1)  // amarillo
	);
	p3->setMass(0.1);
	_forceRegistry->addRegistry(p3, gravity);
	_entities.push_back(p3);


	Particle* p4 = new Particle(
		Vector3(-5, 10, 0), Vector3(0, 0, 0),
		Vector3(0, 0, 0), Vector4(1, 0, 1, 1)  // rosa
	);
	p4->setMass(10);
	_forceRegistry->addRegistry(p4, gravity);
	_entities.push_back(p4);

	//Particle* p2 = new Particle(
	//	Vector3(5, 10, 0), Vector3(0, 0, 0),
	//	Vector3(0, -9.8, 0), Vector4(0, 1, 0, 1)  // Verde
	//);
	//_entities.push_back(p2);

}

void Scene4::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
	case '1':
	{

		break;
	}
	default:
		break;
	}
}