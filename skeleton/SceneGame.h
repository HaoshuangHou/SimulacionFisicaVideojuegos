#pragma once
#include "Scene.h"

class ExplosionGenerator;
class GravityGenerator;
class WindGenerator;

class SceneGame: public Scene
{
protected:
	ExplosionGenerator* _explosionGenerator = nullptr;
	GravityGenerator* _gravityGenerator = nullptr;
	WindGenerator* _windGenerator = nullptr;
public:
	SceneGame();
	virtual ~SceneGame();
	virtual void init() override;
	virtual void handleInput(unsigned char key) override;

	void update(double t) override;
	void enter() override;
	void exit() override;

private:
	void setupCamera();
	void createCanon();
	void createTarget();
	void createPlatforms();
	void createWindZone();
	void shootProjectile();
	void updateCanonAngle();
	void updateProjectilePower();
	void checkWinCondition();
	void resetProjectile();
	void showGameInfo();

	Particle* _canon;
	Particle* _target;
	std::vector<Particle*> _platforms;
	std::vector<Projectil*> _projectiles;
	Projectil* _currentProjectile;

	float _canonAngle;           
	float _projectilePower;
	int _projectilesRemaining;
	int _maxProjectiles;
	bool _windActive;
	float _windStrength;
	bool _gameWon;
	float _gameTime;

	const float MAX_ANGLE = 3.14159f * 0.4f; // 72 grados
	const float MIN_ANGLE = -3.14159f * 0.4f; // -72 grados
	const float MAX_POWER = 50.0f;
	const float MIN_POWER = 5.0f;
};

