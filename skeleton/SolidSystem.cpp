#include "SolidSystem.h"
#include "Particle.h"
#include "Generator.h"
#include "RenderUtils.hpp"
#include "ForceRegistry.h"
#include "ForceGenerator.h"
#include "SolidEntity.h"


void SolidSystem::create_model_solid()
{
	_model_solid = new SolidEntity(_physics, _scene, true, Vector3(0, 0, 0), physx::PxBoxGeometry(100, 1, 100), 0, nullptr, Vector4(0.5, 0.5, 0.5, 1));
}

SolidSystem::SolidSystem(physx::PxPhysics* physics, physx::PxScene* scene, const physx::PxVec3& center, float r)
	:_center(center), _radius(r), _model_solid(nullptr),
	_forceRegistry(std::make_unique<ForceRegistry>()), _physics(physics), _scene(scene)
{
}

SolidSystem::~SolidSystem()
{
	_solids.clear();

	if (_model_solid != nullptr) {
		delete _model_solid;
		_model_solid = nullptr;
	}
}

void SolidSystem::update(double dt)
{

	_forceRegistry->updateForces(dt);

	for (auto& s : _solids) {
		s->update(dt);
	}
	
	delete_solid();

	for (auto g : _generators) {
		if (g) {
			auto new_solids = g->generate();
			for (auto& s : new_solids) {

				_solids.push_back(std::unique_ptr<SolidEntity>(s));
				for (auto& force : _forces) {
					_forceRegistry->addRegistry(s, force.get());
				}
				if (!s->isRenderItemValid())
					s->create_renderItem();
			}
			new_solids.clear();
		}
	}
}

void SolidSystem::addGenerator(Generator<SolidEntity>* gen)
{
	if (gen != nullptr) {
		_generators.push_back(gen);
	}
}

void SolidSystem::addForce(ForceGenerator<SolidEntity>* f)
{
	if (f != nullptr) {
		_forces.push_back(std::unique_ptr<ForceGenerator<SolidEntity>>(f));
		for (auto& p : _solids) {
			_forceRegistry->addRegistry(p.get(), f);
		}
	}
}

void SolidSystem::killAll()
{
	for (auto& r : _solids) {
		if (r) {
			r->kill();
		}
	}
}

void SolidSystem::registerAllRenderItems()
{
	for (auto& r : _solids) {
		if (r) {
			r->create_physicsObject();
			if (!r->isRenderItemValid()) {
				r->create_renderItem();
			}
		}
	}
}

void SolidSystem::deregisterAllRenderItems()
{
	for (auto& r : _solids) {
        if (r) {
			if (physx::PxRigidActor* actor = r->getActor()) {
				if (physx::PxScene* scene = actor->getScene())
					scene->removeActor(*actor);
				actor->release();
			}
            r->deregisterRenderItem();
        }
    }
}

void SolidSystem::delete_solid()
{
	_solids.remove_if([this](std::unique_ptr<SolidEntity>& p) {
		if (!p->is_alive() || isSolidOutsideArea(p.get())) {
			_forceRegistry->clearSolid(p.get());
			return true;
		}
		});
}

bool SolidSystem::isSolidOutsideArea(SolidEntity* particle)
{
	if (!particle) return true;

	const Vector3 particlePos = particle->getTransform().p;
	const Vector3 delta = particlePos - _center;
	const float distance = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
	return distance > _radius;
}