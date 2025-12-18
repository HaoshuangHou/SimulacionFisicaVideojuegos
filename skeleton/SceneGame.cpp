#include "SceneGame.h"
#include "GravityGenerator.h"
#include "WindGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionGenerator.h"
#include "ForceParticleSystem.h"
#include "RenderUtils.hpp"

#include "FireParticleSystem.h"
#include "FogParticleSystem.h"
#include "TapParticleSystem.h"
#include "FireworkParticleSystem.h"

#include "SolidEntity.h"
#include "SolidProjectil.h"
#include "BubbleSystem.h"
#include "AlgaeParticleSystem.h"
#include "SceneCollisionCallback.h"

#include <iostream>
#include <sstream>

SceneGame::SceneGame()
	:Scene(),_target(nullptr),
	_windGenerator(nullptr), _gravityGenerator(nullptr), _explosionGenerator(nullptr), 
	_fireSystem(nullptr), _fireworkSystem(nullptr), _algaeSystem(),
	_windParticleSystem(nullptr), _whirlwindParticleSystem(nullptr),
	_projectilePower(20.0f),_projectilesRemaining(8), _maxProjectiles(8),
	_windActive(false), _whirlwindActive(false),_gameWon(false),_gameOver(false), _explosionTimer(0)
{}

SceneGame::~SceneGame()
{
	_activeProjectiles.clear();
	delete _shooter;
	delete _targetFish;
	_shooter = nullptr;
	_targetFish = nullptr;
}

#pragma region Scene
void SceneGame::init(physx::PxPhysics* physics, physx::PxScene* scene)
{
	initLevels();

	_gPhysics = physics;
	_gScene = scene;

	collisionCallback = new SceneCollisionCallback(this);
	_gScene->setSimulationEventCallback(collisionCallback);

	_text = "FLECHAS Girar / Potencia | SPACE Disparar | 1 Viento | 2 Torbellino ";
	clearScene();

	_gravityGenerator = new GravityGenerator<SolidEntity>(Vector3(0, -9.8, 0));

	_gameWon = false;
	_gameOver = false;
	_projectilesRemaining = _maxProjectiles;
	_windActive = false;
	_whirlwindActive = false;

	loadLevel(_currentLevel);
}

void SceneGame::update(double t)
{

	cleanupDeadProjectiles();

	if (_forceRegistry) {
		_forceRegistry->updateForces(t);
	}

	updateGameState(t);

	if (_gameWon) {
		if (_explosionTimer > 0.0) {
			_explosionTimer -= t;
		}
		else if (_explosionGenerator) {
			_explosionGenerator->active(true);
		}
	}

	Scene::update(t);
}

void SceneGame::enter()
{
	current_projection_mode = PROJ_ORTHOGRAPHIC;
	Scene::enter();
	setupCamera();
	_shooter->create_renderItem();
	_targetFish->create_renderItem();
}

void SceneGame::exit()
{
	current_projection_mode = PROJ_PERSPECTIVE;
	Scene::exit();
	if (GetCamera()) GetCamera()->resetCamera();
	_shooter->deregisterRenderItem();
	_targetFish->deregisterRenderItem();
}

void SceneGame::loadLevel(int index)
{
	if (index < 0 || index >= _levels.size()) return;
	clearScene();
	LevelData& lvl = _levels[index];
	_currentLevel = index;

	_posShooter = lvl.shooterPos;
	_posTarget = lvl.targetPos;
	_posTargetFish = lvl.targetFishPos;

	_shooter = new Fish(getRelativePosition(_posShooter.x, _posShooter.y), { 1,0.5,0.2,1 });
	createGround(100.0f, 100.0f, 2.0f);
	createTarget(getRelativePosition(_posTarget.x, _posTarget.y), 2.0f);
	_targetFish = new Fish(getRelativePosition(_posTargetFish.x, _posTargetFish.y), { 0.6f,0.45f,0.85f,1.0f }, 0.6f);

	createPlatforms(lvl.platforms);
	createForceZones(lvl.forceZones);
	createAlgaeField();

	//createGameObjects();

	if (!_forceRegistry)
		_forceRegistry = new ForceRegistry();


	for (auto& f : lvl.forces) {
		switch (f.type) {
		case ForceType::WIND:
			_windGenerator = new WindGenerator<SolidEntity>(f.pos, f.radius, f.dir * 50, 0.3);
			_windParticleSystem = new ForceParticleSystem(
				new WindGenerator<Particle>(f.pos, f.radius, f.dir * 50, 0.3),
				Vector4(0.6f, 0.7f, 0.8f, 0.45f),
				false
			);
			addParticleSystem(_windParticleSystem);
			_windParticleSystem->setVisible(false);
			break;

		case ForceType::WHIRLWIND:
			_whirlwindGenerator = new WhirlwindGenerator<SolidEntity>(f.pos, f.radius, 1.8f, 0.2f, 0.05f, true);
			_whirlwindParticleSystem = new ForceParticleSystem(
				new WhirlwindGenerator<Particle>(f.pos, f.radius, 1.8f, 0.2f, 0.05f, true),
				Vector4(0.65f, 0.6f, 0.9f, 0.5f),
				true
			);
			addParticleSystem(_whirlwindParticleSystem);
			_whirlwindParticleSystem->setVisible(false);
			break;
		}
	}

	const Vector3 aguaPos = getRelativePosition(0.5f, 0.7f);
	bubbleFloat = new BuoyancyForceGenerator<SolidEntity>(projectilSize / 2, 0.2, 4.0, aguaPos.y);

	_explosionGenerator = new ExplosionGenerator<Particle>(getRelativePosition(_posTarget.x, _posTarget.y), 5.0f, 100, 5);

}

SolidProjectil* SceneGame::getProjectileFromActor(physx::PxActor* const  actor)
{
	for (auto p : _activeProjectiles) {
		if (p && p->getActor() == actor)
			return p;
	}
	return nullptr;
}
SolidEntity* SceneGame::getTargetFromActor(physx::PxActor* const actor)
{
	if (_target && _target->getActor() == actor)
		return _target;
	return nullptr;
}

void SceneGame::onProjectileHitTarget(SolidProjectil* proj, SolidEntity* target)
{
	if (!proj || !target) return;
	proj->setAlive(false); 
	_gameWon = true;

	_fireSystem = new FireParticleSystem(target->getPos(), 5.0f, Vector4(0.1f, 0.9f, 0.8f, 1.0f), Vector4(0.0f, 0.2f, 0.4f, 0.0f), 1.5f);
	_particleSystems.push_back(_fireSystem);
	_target->kill();
	_target = nullptr;
}

void SceneGame::initLevels()
{
	_levels.clear();

	_levels.push_back({
		{0.05f,0.2f},  //shooter pos
		{0.6f,0.7f},  // target Pos
		{0.59f,0.7f},  //TargetFish pos
		// Plataformas
		{
			{getRelativePosition(0.1f,0.05f), Vector3(4,0.5,3), Vector4(0.6f,0.6f,0.65f,1.0f)},
			{getRelativePosition(0.7f,0.15f), Vector3(3,0.5,2.5), Vector4(0.55f,0.6f,0.6f,1.0f)}
		},
		// Zonas de fuerza
		{
			{getRelativePosition(0.1f,0.7f), Vector3(2.5f,2.5f,2.5f), ForceType::WIND},
			{getRelativePosition(0.7f,0.95f), Vector3(2.5f,2.5f,2.5f), ForceType::WHIRLWIND}
		},
		// Fuerzas ambientales 
		{
			// Viento a la izquierda, arriba, hacia la derecha
			{ getRelativePosition(0.2f, 0.7f), Vector3(1,0,0), 5.0f, ForceType::WIND },
				// Torbellino cerca del suelo, gira a la derecha y sube un poco
			{getRelativePosition(0.7f, 0.1f), Vector3(0.5f,1.0f,0.5f), 4.0f, ForceType::WHIRLWIND}
		}
		});
}

void SceneGame::clearScene()
{
	_activeProjectiles.clear();
	_particles.clear();

	removePacticleSystem(_fireSystem);
	removePacticleSystem(_fireworkSystem);
	for (auto a : _algaeSystem) {
		removePacticleSystem(a);
	}

	if(_windParticleSystem)_windParticleSystem->setVisible(false);
	if(_whirlwindParticleSystem)_whirlwindParticleSystem->setVisible(false);

	if (_explosionGenerator) {
		_explosionGenerator->active(false);
	}

	if (_forceRegistry) _forceRegistry->clear();

	delete _shooter;
	delete _targetFish;
	_shooter = nullptr;
	_targetFish = nullptr;

	removeSolid(_target);
	_target = nullptr;
}

#pragma endregion

#pragma region Input
void SceneGame::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
	case ' ': 
	{
		shoot();
		break;
	}
	case '1':
		toggleForce(WIND);
		break;

	case '2':
		toggleForce(WHIRLWIND);
		break;

	default:
		break;
	}
}

void SceneGame::handleSpecialInput(int key)
{
	if (_gameWon||_gameOver) return;
	switch (key) {
	case GLUT_KEY_LEFT:
		updateShootAngle(-0.1f);
		break;
	case GLUT_KEY_RIGHT:
		updateShootAngle(0.1f);
		break;
	case GLUT_KEY_UP:
		updateProjectilePower(2.0f);
		break;
	case GLUT_KEY_DOWN:
		updateProjectilePower(-2.0f);
		break;

	default:
		break;
	}
}
#pragma endregion

#pragma region Shoot
void SceneGame::shoot()
{
	if (!_shooter || !_gScene || !_gPhysics) {
		std::cerr << "[Error] Shooter o PhysX no inicializados!" << std::endl;
		return;
	}
	if (_projectilesRemaining <= 0 || !_shooter || _gameWon)
		return;

	Vector3 dir = _shooter->getShootDirection();
	dir.normalize();
	const Vector3 ini_pos = _shooter->getPosition();

	SolidProjectil* newProjectile = new SolidProjectil(_gPhysics, _gScene, ini_pos, _projectilePower * dir, Vector4(0.8f, 0.9f, 1.0f, 0.8f));
	newProjectile->setLifeTime(10.0f);
	addEntityWithRenderItem(newProjectile);

	if (_gravityGenerator)
		_forceRegistry->addRegistry(newProjectile, _gravityGenerator);
	if (_windGenerator && _windActive) {
		_forceRegistry->addRegistry(newProjectile, _windGenerator);
	}
	if (_whirlwindGenerator && _whirlwindActive) {
		_forceRegistry->addRegistry(newProjectile, _whirlwindGenerator);
	}
	if (bubbleFloat) {
		_forceRegistry->addRegistry(newProjectile, bubbleFloat);
	}
	_activeProjectiles.push_back(newProjectile);
	_projectilesRemaining--;

	std::cout << "_activeProjectiles.size()=" << _activeProjectiles.size()
		<< " | remaining=" << _projectilesRemaining << std::endl;
}

void SceneGame::updateShootAngle(float delta)
{
	if (!_shooter) return;

	float newAngle = _shooter->getAngle() + delta;

	if (newAngle > MAX_ANGLE) newAngle = MAX_ANGLE;
	if (newAngle < MIN_ANGLE) newAngle = MIN_ANGLE;

	_shooter->setAngle(newAngle);
}

void SceneGame::updateProjectilePower(float delta)
{
	_projectilePower += delta;

	if (_projectilePower > MAX_POWER) _projectilePower = MAX_POWER;
	if (_projectilePower < MIN_POWER) _projectilePower = MIN_POWER;
}

void SceneGame::cleanupDeadProjectiles()
{
	_activeProjectiles.erase(
		std::remove_if(
			_activeProjectiles.begin(),
			_activeProjectiles.end(),
			[](SolidProjectil* p)
			{
				return !p || !p->is_alive();
			}
		),
		_activeProjectiles.end()
	);
}
#pragma endregion

#pragma region GameInfo
void SceneGame::updateGameState(double t)
{
	if (_gameWon) {
		_text = "VICTORIA | Presiona R para reiniciar";

		return;
	}
	if (_projectilesRemaining <= 0 && !_gameWon) {
		if (!_gameOver) {
			_text = "GAME OVER： Sin proyectiles| Presiona R para reiniciar";
			_gameOver = true;
		}
		return;
	}
	std::stringstream ss;
	ss << "FLECHAS Girar / Potencia | SPACE Disparar | 1 Viento | 2 Torbellino  "
		<< "  Potencia " << _projectilePower
		<< " | Proyectiles " << _projectilesRemaining << "/" << _maxProjectiles;
	_text = ss.str();
}

#pragma endregion

#pragma region GameObjectPosition

void SceneGame::repositionObjects()
{
	if (_shooter) {
		_shooter->setPosition(getRelativePosition(_posShooter.x, _posShooter.y));
	}
	
	if (_target) {
		_target->setPosition(getRelativePosition(_posTarget.x, _posTarget.y));
		_targetFish->setPosition(getRelativePosition(_posTargetFish.x, _posTargetFish.y));
	}
	if (_ground) {
		_ground->setPosition(getRelativePosition(0.5, 0.05));
	}
}
#pragma endregion

#pragma region GameObjects
void SceneGame::createTarget(const Vector3& pos, float radius)
{
	physx::PxMaterial* material = _gPhysics->createMaterial(0.5f, 0.5f, 0.1f);
	_target =createRigidEntity(false,                     
		physx::PxVec3(pos.x, pos.y, pos.z),
		physx::PxSphereGeometry(radius),
		9.0,                        // densidad
		material,
		Vector4(0.3f, 1.0f, 0.9f, 0.5f));
}

void SceneGame::createGround(float width, float depth, float height)
{
	physx::PxMaterial* material = _gPhysics->createMaterial(0.8f, 0.7f, 0.1f);

	_ground= createRigidEntity(
		false,                       
		getRelativePosition(0.5, 0.05),
		physx::PxBoxGeometry(width / 2.0f, height / 2.0f , depth / 2.0f),
		0.0,                       
		material,
		Vector4(0.8f, 0.7f, 0.4f, 1.0f) 
	);
}

void SceneGame::createPlatforms(const std::vector<PlatformData>& platforms)
{
	if (!_gPhysics || !_gScene) return;
	physx::PxMaterial* platformMaterial = _gPhysics->createMaterial(0.4f, 0.4f, 0.4f);

	for (auto& cube : platforms)
	{
		createRigidEntity(
			false,
			cube.pos,
			physx::PxBoxGeometry(cube.size.x / 2, cube.size.y / 2, cube.size.z / 2),
			0.0,
			platformMaterial,
			cube.color
		);
	}
}

void SceneGame::createZone(const Vector3& pos,const Vector3& size,physx::PxMaterial* material,ForceType forceType)
{
	SolidEntity* zone = createRigidEntity(
		false,
		pos,
		physx::PxBoxGeometry(size.x / 2, size.y / 2, size.z / 2),
		0.0f,
		material,
		Vector4(0.2f, 0.6f, 1.0f, 0.25f)
	);
	zone->create_physicsObject();
	collisionCallback->addForceZones(zone->getActor(), forceType);
}

void SceneGame::createForceZones(const std::vector<ForceZoneData>& zones)
{
	physx::PxMaterial* zoneMaterial = _gPhysics->createMaterial(0.4f, 0.4f, 0.4f);

	for (auto& z : zones)
	{
		createZone(z.pos, z.size, zoneMaterial, z.type);
	}
}

void SceneGame::createAlgaeField()
{
	std::vector<float> relX = { 0.1f, 0.4f, 0.6f, 0.75f, 0.8 };


	for (auto& rx : relX) {
		AlgaeParticleSystem* a = new AlgaeParticleSystem(getRelativePosition(rx, 0.0f), 50);
		_particleSystems.push_back(a);
	}
}

//void SceneGame::createGameObjects()
//{
//	_posShooter = physx::PxVec2(0.05,0.2);
//	_posTarget = physx::PxVec2(0.6, 0.7);
//	_posTargetFish = physx::PxVec2(0.59, 0.7);
//
//	if(!_target)createTarget(getRelativePosition(_posTarget.x, _posTarget.y), 2.0f); 
//	if (_gameOver || _gameWon) return;
//
//	_shooter = new Fish(getRelativePosition(_posShooter.x, _posShooter.y), { 1.0f, 0.5f, 0.2f, 1.0f });
//
//	createGround(100.0f, 100.0f, 2.0f);   // suelo plano a Y=-1
//
//	_targetFish = new Fish(getRelativePosition(_posTargetFish.x, _posTargetFish.y), { 0.6f, 0.45f, 0.85f, 1.0f }, 0.6);
//	_targetFish->rotate(60);
//
//	//createPlatform();
//	createAlgaeField();
//	createZones();
//
//	_fireworkSystem = new FireworkParticleSystem(getRelativePosition(0.5, 0.1));
//	addParticleSystem(_fireSystem);
//}
#pragma endregion

#pragma region Forces
void SceneGame::toggleForce(ForceType forceType)
{
	switch (forceType) {
	case WIND:
		setForceActive(WIND, !_windActive);
		break;

	case WHIRLWIND:
		setForceActive(WHIRLWIND, !_whirlwindActive);
		break;
	default:
		break;
	}	
}

void SceneGame::setForceActive(ForceType forceType, bool active)
{
	switch (forceType) {

	case WIND:
		if (!_windGenerator || !_windParticleSystem) return;

		if (_windActive == active) return;

		_windActive = active;
		_windParticleSystem->setVisible(active);
		applyForceToAllProjectiles(_windGenerator, active);
		break;

	case WHIRLWIND:
		if (!_whirlwindGenerator || !_whirlwindParticleSystem) return;

		if (_whirlwindActive == active) return;

		_whirlwindActive = active;
		_whirlwindParticleSystem->setVisible(active);
		applyForceToAllProjectiles(_whirlwindGenerator, active);
		break;
	}
}


void SceneGame::applyForceToAllProjectiles(ForceGenerator<SolidEntity>* forceGenerator, bool active)
{
	if (!forceGenerator) return;

	for (auto& projectile : _activeProjectiles) {
		if (projectile) {
			if (active) {
				_forceRegistry->addRegistry(projectile, forceGenerator);
			}
			else {
				_forceRegistry->remove(projectile, forceGenerator);
			}
		}
	}
}

#pragma endregion