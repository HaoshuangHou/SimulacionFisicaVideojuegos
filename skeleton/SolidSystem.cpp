//#include "SolidSystem.h"
//#include "Particle.h"
//#include "Generator.h"
//#include "RenderUtils.hpp"
//#include "ForceRegistry.h"
//#include "ForceGenerator.h"
//
//void SolidSystem::create_model_solid()
//{
//}
//
//SolidSystem::SolidSystem(Scene* scene, const physx::PxVec3& center, float r)
//{
//}
//
//SolidSystem::~SolidSystem()
//{
//	_solids.clear();
//
//	if (_model_solid != nullptr) {
//		delete _model_solid;
//		_model_solid = nullptr;
//	}
//}
//
////void SolidSystem::update(double dt)
////{
////	for (auto g : _generators) {
////		if (g) {
////			auto new_particles = g->generateP();
////			for (auto& new_p : new_particles) {
////				_particles.push_back(std::unique_ptr<Particle>(new_p));
////				for (auto& force : _forces) {
////					_forceRegistry->addRegistry(new_p, force.get());
////				}
////			}
////			new_particles.clear();
////		}
////	}
////}
////
////void SolidSystem::addGenerator(ParticleGen* gen)
////{
////	if (gen != nullptr) {
////		_generators.push_back(gen);
////	}
////}
////
////void SolidSystem::addForce(ForceGenerator* f)
////{
////	if (f != nullptr) {
////		_forces.push_back(std::unique_ptr<ForceGenerator>(f));
////		for (auto& p : _particles) {
////			_forceRegistry->addRegistry(p.get(), f);
////		}
////	}
////}
////
////void SolidSystem::registerAllRenderItems()
////{
////	for (auto& particle : _solids) {
////		if (particle && !particle->isRenderItemValid()) {
////			particle->create_renderItem(); 
////		}
////	}
////}
////
////void SolidSystem::deregisterAllRenderItems()
////{
////	for (auto& particle : _solids) {
////		if (particle) particle->deregisterRenderItem();
////	}
////}
////
////bool SolidSystem::isParticleOutsideArea(Particle* particle)
////{
////	if (!particle) return true;
////
////	const Vector3 particlePos = particle->getTransform().p;
////	const Vector3 delta = particlePos - _center;
////	const float distance = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
////	return distance > _radius;
////}