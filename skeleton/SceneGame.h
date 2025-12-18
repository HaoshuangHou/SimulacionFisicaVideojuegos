#pragma once
#include "Scene.h"
#include "Projectil.h"
#include "Fish.h"
#include <memory>

class FireworkParticleSystem;
class FireParticleSystem;
class TapParticleSystem;
class FogParticleSystem;
class ForceParticleSystem;
class SolidProjectil;
class AlgaeParticleSystem;
class SceneCollisionCallback;

enum ForceType {
	WIND, WHIRLWIND, NUM_FORCE
};

struct ForceZoneData {
	Vector3 pos;
	Vector3 size;
	ForceType type;
};

struct ForceData {
	Vector3 pos;
	Vector3 dir;   
	float radius;
	ForceType type;
};

struct PlatformData {
	Vector3 pos;
	Vector3 size;
	Vector4 color;
};

struct LevelData {
	physx::PxVec2 shooterPos;
	physx::PxVec2 targetPos;
	physx::PxVec2 targetFishPos;
	std::vector<PlatformData> platforms;
	std::vector<ForceZoneData> forceZones;
	std::vector<ForceData> forces;
};

class SceneGame : public Scene
{
	std::vector<LevelData> _levels;
	int _currentLevel = 0;

public:
	SceneGame();
	virtual ~SceneGame();

	void init(physx::PxPhysics* physics, physx::PxScene* scene) override;
	void handleInput(unsigned char key) override;
	void handleSpecialInput(int key) override;

	void update(double t) override;
	void enter() override;
	void exit() override;

	void loadLevel(int index);

	SolidProjectil* getProjectileFromActor(physx::PxActor* const actor);
	SolidEntity* getTargetFromActor(physx::PxActor* actor);
	void onProjectileHitTarget(SolidProjectil* proj, SolidEntity* target);

	void setForceActive(ForceType forceType, bool active);

protected:
	void initLevels();
	void clearScene();
	void createGameObjects();

	void shoot();
	void updateShootAngle(float delta);
	void updateProjectilePower(float delta);

	void cleanupDeadProjectiles();

	void updateGameState(double t);

	void toggleForce(ForceType forceType);
	
	void applyForceToAllProjectiles(ForceGenerator<SolidEntity>* forceGenerator, bool active);

	virtual	void repositionObjects() override;
	
	void createTarget(const Vector3& pos, float radius);
	void createGround(float width, float depth, float height = 0.0f);
	
	virtual void createPlatforms(const std::vector<PlatformData>& platforms);
	virtual void createZone(const Vector3& pos,const Vector3& halfExtents,physx::PxMaterial* material,ForceType forceType);
	virtual void createForceZones(const std::vector<ForceZoneData>& zones);
	virtual void createAlgaeField();

	Fish* _shooter = nullptr;
	float projectilSize = 0.6f;
	SolidEntity* _target = nullptr;
	Fish* _targetFish = nullptr;
	SolidEntity* _ground = nullptr;
	physx::PxVec2 _posShooter;
	physx::PxVec2 _posTarget;
	physx::PxVec2 _posTargetFish;
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

	// Fuerzas
	ExplosionGenerator<Particle>* _explosionGenerator;
	WindGenerator<SolidEntity>* _windGenerator;
	GravityGenerator<SolidEntity>* _gravityGenerator;
	WhirlwindGenerator<SolidEntity>* _whirlwindGenerator;
	BuoyancyForceGenerator<SolidEntity>* bubbleFloat;

	//sistemas de particulas
	FireworkParticleSystem* _fireworkSystem;
	FireParticleSystem* _fireSystem;
	std::vector<AlgaeParticleSystem*> _algaeSystem;
	//FogParticleSystem* _fog;

	double _explosionTimer;
	Vector3 _victoryPos;

	ForceParticleSystem* _windParticleSystem;
	ForceParticleSystem* _whirlwindParticleSystem;

	bool _windActive;
	bool _whirlwindActive;

	SceneCollisionCallback* collisionCallback = nullptr;
};