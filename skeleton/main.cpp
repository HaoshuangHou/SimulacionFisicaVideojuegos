#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <iostream>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

//mis Clases
#include "callbacks.hpp"

#include "Particle.h"


std::string display_text = "This is a test";
std::vector<Particle*> particles;

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc); //crear la escena


	//RegisterRenderItem(new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(0, 0, 0), Vector4(1, 1, 1, 1)));
	//RegisterRenderItem(new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(10, 0, 0), Vector4(1, 0, 0, 1)));
	//RegisterRenderItem(new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(0, 10, 0), Vector4(0, 0, 1, 1)));
	//RegisterRenderItem(new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(0, 0, 10), Vector4(0, 1, 0, 1)));
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	for (auto it = particles.begin(); it != particles.end(); ++it)
	{
		if (*it != nullptr) {
			(*it)->update(t);
		}
	}
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	particles.clear();
	DeregisterAllRenderItem();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case '1':
	{
		Particle* p = new Particle(Vector3(0, 10, 0), Vector3(5, 10, 0),
			Vector3(0, -9.8, 0), EULER);
		particles.push_back(p);
		break;
	} 
	case '2':
	{
		Particle* p = new Particle(Vector3(0, 10, 0), Vector3(5, 10, 0),
			Vector3(0, -9.8, 0), EULER_SEMI_IMPLICITO);
		particles.push_back(p);
		break;
	}
	case '3':
	{
		Particle* p = new Particle(Vector3(0, 10, 0), Vector3(5, 10, 0),
			Vector3(0, -9.8, 0), VERLET);
		particles.push_back(p);
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}