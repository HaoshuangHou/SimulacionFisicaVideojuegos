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
	_particles.remove_if([this](Particle* p) {
		if (!p->is_alive() || isParticleOutsideArea(p)) {
			delete p;
			return true;
		}
		return false;
		});
}

ParticleSystem::ParticleSystem(const Vector3& center, float r)
	:_center(center), _radius(r), _model_particle(nullptr)
{}

ParticleSystem::~ParticleSystem()
{
	for (auto p : _particles) {
		delete p;
	}
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
	for (Particle* p : _particles) {
		p->update(dt);
	}
		
	delete_particle();

	for (auto g : _generators) {
		auto new_particles = g->generateP();
		_particles.splice(_particles.end(), new_particles);
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
	for (auto particle : _particles) {
		if (particle->getRenderItem() == nullptr) {
			particle->create_renderItem();
		}
		else {
			RegisterRenderItem(particle->getRenderItem());
		}
	}
}

void ParticleSystem::deregisterAllRenderItems()
{
	for (auto particle : _particles) {
		if (particle->getRenderItem() != nullptr) {
			DeregisterRenderItem(particle->getRenderItem());
		}
	}
}

bool ParticleSystem::isParticleOutsideArea(Particle* particle)
{
	if (!particle) return true;

	Vector3 particlePos = particle->getTransform().p;
	Vector3 delta = particlePos - _center;
	float distance = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
	return distance > _radius;
}