#include "Scene5.h"

void Scene5::create_slinky()
{
	for (int i = 0; i < 6; i++) {

		Particle* p = new Particle(Vector3(25, 40 +4*i, 0), Vector3(0, 0, 0),
			Vector3(0, 0, 0), Vector4(1, 0, 1, 1), 2.0f, 0.5);
		p->setLifeTime(-1);

		_slinky_particles.push_back(p);
		_particles.push_back(p);
	}

	for (int i = 0; i < 5; i++) {
		ElasticBandForceGenerator<Particle>* e = new ElasticBandForceGenerator<Particle>(8, 4, _slinky_particles[i + 1]);
		_forceRegistry->addRegistry(_slinky_particles[i], e);
		_forceRegistry->addRegistry(_slinky_particles[i], _gravityGenerator);
	}
}

void Scene5::init(physx::PxPhysics* physics, physx::PxScene* scene)
{
	_gPhysics = physics;
	_gScene = scene;

	_text = "ESCENA 5: MUELLES, SIGUIENTE ESCENA(Q)";
	_gravityGenerator = new GravityGenerator<Particle>(Vector3(0, -9.8, 0));

	//Muelle: una particula unida a una posicion estatica
	pEstatico = new Particle(CUBE,Vector4(1, 0, 0, 1), Vector3(0, 50, 0));
	pEstatico->setLifeTime(-1);
	_particles.push_back(pEstatico);


	p1_muelle = new Particle(
		Vector3(20, 50, 0), Vector3(0, 0, 0),
		Vector3(0, 0, 0), Vector4(1, 0, 1, 1),
		1.0f, 0.85);
	p1_muelle->setLifeTime(-1);

	_fSpring = new SpringForceGenerator<Particle>(10, 10, pEstatico);
	_forceRegistry->addRegistry(p1_muelle, _fSpring);
	_particles.push_back(p1_muelle);

	//Goma Elastica
	p1_goma= new Particle(
		Vector3(0, 40, 0), Vector3(0, 0, 0),
		Vector3(0, 0, 0), Vector4(1, 1, 0, 1),
		1.0f, 0.85);
	p1_goma->setLifeTime(-1);

	p2_goma = new Particle(
		Vector3(10, 40, 0), Vector3(0, 0, 0),
		Vector3(0, 0, 0), Vector4(0, 1, 1, 1),
		1.0f, 0.85);
	p2_goma->setLifeTime(-1);

	_particles.push_back(p1_goma);
	_particles.push_back(p2_goma);

	_fElastic1 = new ElasticBandForceGenerator<Particle>(20, 10, p1_goma);
	_fElastic2 = new ElasticBandForceGenerator<Particle>(20, 10, p2_goma);

	_forceRegistry->addRegistry(p1_goma, _fElastic2);
	_forceRegistry->addRegistry(p2_goma, _fElastic1);


	//Flotacion
	Particle* agua = new Particle(CUBE, Vector4(0, 0, 1, 1), Vector3(0, 18.0f, 0));
	agua->setLifeTime(-1);
	agua->setTam({ 10, 2, 10 });

	_particles.push_back(agua);

	double const waterHeight = 21.0f;
	double const cubeHeight = 1.0f;
	double const volume = 1.0f;

	pCubo = new Particle(CUBE,Vector4(1, 0, 1, 1), Vector3(0, 25.0f, 0));
	pCubo->setMass(900.0f);
	pCubo->setTam(0.5f);
	pCubo->setDamping(0.3f);
	pCubo->setLifeTime(-1);
	_particles.push_back(pCubo);

	BuoyancyForceGenerator<Particle>* b1 = new BuoyancyForceGenerator<Particle>(cubeHeight, volume, 1000.0f, waterHeight);
	_forceRegistry->addRegistry(pCubo, b1);
	_forceRegistry->addRegistry(pCubo, _gravityGenerator);

	create_slinky();
}

void Scene5::enter()
{
	Scene::enter();
}

void Scene5::exit()
{
	Scene::exit();
}

void Scene5::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
		case '1':
		{
			p1_muelle->setPosition(Vector3(0, 30, 0));
			break;
		}
		case '2':
		{
			_fElastic1->set_k(500);
			_fElastic2->set_k(500);
			_fSpring->set_k(500);
			break;
		}
		case '3':
		{
			_fElastic1->set_k(1);
			_fElastic2->set_k(1);
			_fSpring->set_k(1);
			break;
		}
		case 'M':
		{
			pCubo->setMass(500);
			break;
		}
		case 'N':
		{
			pCubo->setMass(1000);
			break;
		}
		case 'G':
		{
			_forceRegistry->addRegistry(p1_muelle, _gravityGenerator);
			_forceRegistry->addRegistry(p2_goma, _gravityGenerator);
			break;
		}
		case 'H':
		{
			_forceRegistry->remove(p1_muelle, _gravityGenerator);
			_forceRegistry->remove(p2_goma, _gravityGenerator);
			break;
		}
		case '4':
		{
			Particle* a = new Particle(
				Vector3(-10, 40, 0), Vector3(0, 0, 0), Vector3(0, 0, 0),
				Vector4(1, 0.5, 0, 1), 1.0f, 0.85f
			);
			a->setLifeTime(-1);

			Particle* b = new Particle(
				Vector3(0, 40, 0), Vector3(0, 0, 0), Vector3(0, 0, 0),
				Vector4(1, 0.5, 0, 1), 1.0f, 0.85f
			);
			b->setLifeTime(-1);
			addEntityWithRenderItem(a);
			addEntityWithRenderItem(b);

			ElasticBandForceGenerator<Particle>* f1 = new ElasticBandForceGenerator<Particle>(
				20.0f, 
				6.0f,  
				b        
			);
			ElasticBandForceGenerator<Particle>* f2 = new ElasticBandForceGenerator<Particle>(
				20.0f,
				6.0f,
				a
			);
			_forceRegistry->addRegistry(a, f1);
			_forceRegistry->addRegistry(b, f2);
			_forceRegistry->addRegistry(b, _gravityGenerator);

		}
		default: {
			break;
		}
	}
}