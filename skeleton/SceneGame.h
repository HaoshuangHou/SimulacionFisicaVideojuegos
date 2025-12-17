#pragma once
#include "Scene.h"
#include "Projectil.h"
#include "Shooter.h"
#include <memory>

class FireworkParticleSystem;
class FireParticleSystem;
class TapParticleSystem;
class FogParticleSystem;
class ForceParticleSystem;
class SolidProjectil;

enum ForceType {
	WIND, WHIRLWIND, EXPLOSION, NUM_FORCE
};
class SceneGame : public Scene
{
public:
	SceneGame();
	virtual ~SceneGame();

	void init(physx::PxPhysics* physics, physx::PxScene* scene) override;
	void handleInput(unsigned char key) override;
	void handleSpecialInput(int key) override;

	void update(double t) override;
	void enter() override;
	void exit() override;

private:
	void clearScene();
	void createGameObjects();

	void shoot();
	void updateShootAngle(float delta);
	void updateProjectilePower(float delta);
	void checkProjectileState();

	void updateGameState(double t);

	Shooter* _shooter = nullptr;
	Particle* _target;
	std::list<SolidProjectil*> _activeProjectiles;

	// Estado del juego
	float _projectilePower;
	int _projectilesRemaining;
	int _maxProjectiles;
	bool _gameWon;
	bool _gameOver;

	// Constantes para limitar los valores
	const float MAX_ANGLE = 3.14159f * 0.6f;
	const float MIN_ANGLE = -3.14159f * 0.1f; 
	const float MAX_POWER = 50.0f;
	const float MIN_POWER = 5.0f;

	virtual	void repositionObjects() override;

	// Fuerzas
	ExplosionGenerator<Particle>* _explosionGenerator;
	WindGenerator<SolidEntity>* _windGenerator;
	GravityGenerator<SolidEntity>* _gravityGenerator;
	WhirlwindGenerator<SolidEntity>* _whirlwindGenerator;

	//sistemas de particulas
	FireworkParticleSystem* _fireworkSystem;
	FireParticleSystem* _fireSystem;
	FogParticleSystem* _fog;

	double _explosionTimer;
	Vector3 _victoryPos;

	ForceParticleSystem* _windParticleSystem;
	ForceParticleSystem* _whirlwindParticleSystem;

	bool _windActive;
	bool _whirlwindActive;

	void setupForces();
	void toggleForce(ForceType forceType);
	void applyForceToAllProjectiles(ForceGenerator<SolidEntity>* forceGenerator, bool active);
};