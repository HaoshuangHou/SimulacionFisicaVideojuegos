#include "ParticleSystem.h"
#include "Particle.h"
#include "ParticleGen.h"
#include "RenderUtils.hpp"

void ParticleSystem::create_model_particle()
{
	_model_particle = new Particle();
	_model_particle->setAcceleration({ 0,-10,0 });
}
void ParticleSystem::create_model_particle(double tam)
{
	_model_particle = new Particle();
	_model_particle->setTam(tam);
	_model_particle->setAcceleration({ 0,-10,0 });
}
void ParticleSystem::delete_particle()
{
	_particles.remove_if([this](std::unique_ptr<Particle>& p) {
		return  !p->is_alive() || isParticleOutsideArea(p.get());
		});
}

ParticleSystem::ParticleSystem(const Vector3& center, float r)
	:_center(center), _radius(r), _model_particle(nullptr)
{}

ParticleSystem::~ParticleSystem()
{
	_particles.clear();

	for (auto g : _generators) {
		delete g;
	}
	_generators.clear();

	delete _model_particle;
	_model_particle = nullptr;
}

void ParticleSystem::update(double dt)
{
	for (auto& p : _particles) {
		p->update(dt);
	}
		
	delete_particle();

	for (auto g : _generators) {
		auto new_particles = g->generateP();
		for (auto& new_p : new_particles) {
			_particles.push_back(std::unique_ptr<Particle>(new_p));
		}
		new_particles.clear();
	}
}

void ParticleSystem::addGenerator(ParticleGen* gen)
{
	if (gen != nullptr) {
		_generators.push_back(gen);
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