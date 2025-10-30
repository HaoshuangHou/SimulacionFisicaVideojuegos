#include "Scene3.h"
#include "FireParticleSystem.h"
#include "FogParticleSystem.h"
#include "TapParticleSystem.h"
#include "FireworkParticleSystem.h"

void Scene3::init()
{
	_text = "ESCENA 3: SISTEMAS DE PARTICULAS, PASAR DE ESCENA CON Q";
	createParticleSystem();
}

void Scene3::handleInput(unsigned char key)
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


void Scene3::createParticleSystem()
{
	addParticleSystem(new FireParticleSystem());
	addParticleSystem(new FogParticleSystem());
	addParticleSystem(new TapParticleSystem({ -10,20,0 }));
	//addParticleSystem(new FireworkParticleSystem());
}
