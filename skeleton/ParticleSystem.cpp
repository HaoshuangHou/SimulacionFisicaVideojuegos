#include "ParticleSystem.h"
#include "Particle.h"
#include "ParticleGen.h"
#include "RenderUtils.hpp"
#include "ForceRegistry.h"
#include "ForceGenerator.h"

void ParticleSystem::create_model_particle()
{
	_model_particle = new Particle();
}
void ParticleSystem::create_model_particle(double tam)
{
	_model_particle = new Particle();
	_model_particle->setTam(tam);
}
void ParticleSystem::delete_particle()
{
	_particles.remove_if([this](std::unique_ptr<Particle>& p) {
		if (!p->is_alive() || isParticleOutsideArea(p.get())) {
			_forceRegistry->clearParticle(p.get());
			return true;}
		});
}

ParticleSystem::ParticleSystem(const Vector3& center, float r)
	:_center(center), _radius(r), _model_particle(nullptr), 
	_forceRegistry(std::make_unique<ForceRegistry>())
{}

ParticleSystem::~ParticleSystem()
{
	_particles.clear();

	for (auto g : _generators) {
		delete g;
		g = nullptr;
	}
	_generators.clear();
	if (_model_particle != nullptr) {
		delete _model_particle;
		_model_particle = nullptr;
	}
}

void ParticleSystem::update(double dt)
{
	_forceRegistry->updateForces(dt);

	for (auto& p : _particles) {
		p->update(dt);
	}
		
	delete_particle();

	for (auto g : _generators) {
		if (g) {
			auto new_particles = g->generateP();
			for (auto& new_p : new_particles) {
				_particles.push_back(std::unique_ptr<Particle>(new_p));
				for (auto& force : _forces) {
					_forceRegistry->addRegistry(new_p, force.get());
				}
			}
			new_particles.clear();
		}
	}
}

void ParticleSystem::addGenerator(ParticleGen* gen)
{
	if (gen != nullptr) {
		_generators.push_back(gen);
	}
}

void ParticleSystem::addForce(ForceGenerator* f)
{
	if (f != nullptr) {
		_forces.push_back(std::unique_ptr<ForceGenerator>(f));
		for (auto& p : _particles) {
			_forceRegistry->addRegistry(p.get(), f);
		}
	}
}

void ParticleSystem::registerAllRenderItems()
{
	for (auto& particle : _particles) {
		if (particle && !particle->isRenderItemValid()) {
			particle->create_renderItem(); 
		}
	}
}

void ParticleSystem::deregisterAllRenderItems()
{
	for (auto& particle : _particles) {
		if (particle) particle->deregisterRenderItem();
	}
}

bool ParticleSystem::isParticleOutsideArea(Particle* particle)
{
	if (!particle) return true;

	const Vector3 particlePos = particle->getTransform().p;
	const Vector3 delta = particlePos - _center;
	const float distance = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
	return distance > _radius;
}