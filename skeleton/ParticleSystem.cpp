#include "ParticleSystem.h"
#include "Particle.h"
#include "ParticleGen.h"

void ParticleSystem::create_model_particle()
{
	_model_particle = new Particle();
}

void ParticleSystem::delete_particle()
{
	for (auto p : _particles)if (!p->is_alive()) delete p;
}

ParticleSystem::ParticleSystem(): _particles(),_generators(),_model_particle(nullptr)
{

}

void ParticleSystem::update(double dt)
{
	for (Particle* p : _particles) p->update(dt);
	delete_particle();
	for (auto g : _generators) {
		auto new_praticles = g->generateP();
		for (auto p : new_praticles) {
			_particles.push_back(p);
		}
	}
}
