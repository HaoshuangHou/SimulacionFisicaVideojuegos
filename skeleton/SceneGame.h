#pragma once
#include "Scene.h"
#include "Projectil.h"
#include "Shooter.h"

class ExplosionGenerator;
class GravityGenerator;
class WindGenerator;

class SceneGame : public Scene
{
public:
	SceneGame();
	virtual ~SceneGame();

	void init() override;
	void handleInput(unsigned char key) override;
	void handleSpecialInput(int key) override;

	void update(double t) override;
	void enter() override;
	void exit() override;

private:
	void clearScene();
	void createGameObjects();
	void setupForces();

	void shoot();
	void updateShootAngle(float delta);
	void updateProjectilePower(float delta);
	void checkProjectileState();
	void checkGameOver();

	void setupCamera();
	void showGameInfo();
	void updateGameState(double t);

	Shooter* _shooter = nullptr;
	Particle* _target;
	Projectil* _currentProjectile;

	// Estado del juego
	float _projectilePower;
	int _projectilesRemaining;
	int _maxProjectiles;
	bool _gameWon;

	// Fuerzas
	ExplosionGenerator* _explosionGenerator = nullptr;
	WindGenerator* _windGenerator = nullptr;
	GravityGenerator* _gravityGenerator = nullptr;
	bool _windActive;
	float _windStrength;

	// Constantes
	const float MAX_ANGLE = 3.14159f * 0.6f;
	const float MIN_ANGLE = -3.14159f * 0.1f; 
	const float MAX_POWER = 50.0f;
	const float MIN_POWER = 5.0f;


	float _worldWidth, _worldHeight;

	Vector3 getRelativePosition(float relX, float relY, float z = 0.0f) const;
	void updateViewportFromScreen();
	void repositionObjects();
};