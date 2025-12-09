#include "Scene3.h"
#include "FireParticleSystem.h"
#include "FogParticleSystem.h"
#include "TapParticleSystem.h"
#include "FireworkParticleSystem.h"
#include "GravityGenerator.h"

void Scene3::init(physx::PxPhysics* physics, physx::PxScene* scene, physx::PxMaterial* material)
{
	_gPhysics = physics;
	_gScene = scene;
	_gMaterial = material;

	_text = "ESCENA 3: SISTEMAS DE PARTICULAS, SIGUIENTE ESCENA(Q)";
	createParticleSystem();
}

void Scene3::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
	case '1':
	{
		f->createFirework();
		break;
	}
	default:
		break;
	}
}


void Scene3::createParticleSystem()
{
	addParticleSystem(new FireParticleSystem());
	addParticleSystem(new FogParticleSystem());

	TapParticleSystem* t = new TapParticleSystem({ -10,20,0 });
	t->addForce(new GravityGenerator(Vector3(0, -9.8, 0)));
	addParticleSystem(t);
	f = new FireworkParticleSystem(Vector3(0,0,0));
	f->addForce(new GravityGenerator(Vector3(0, -9.8, 0)));
	addParticleSystem(f);
}
