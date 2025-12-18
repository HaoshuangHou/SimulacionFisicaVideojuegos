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
	_windGenerator(nullptr), _gravityGenerator(nullptr), _fogSystem(nullptr), 
	_fireSystem(nullptr), _fireworkSystem1(nullptr), _fireworkSystem2(nullptr), _algaeSystem(),
	_windParticleSystem(nullptr), _whirlwindParticleSystem(nullptr),
	_projectilePower(20.0f),_projectilesRemaining(8), _maxProjectiles(8),
	_windActive(false), _whirlwindActive(false)
{}

SceneGame::~SceneGame()
{
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

	_projectilesRemaining = _maxProjectiles;
	_windActive = false;
	_whirlwindActive = false;

	loadLevel(_currentLevel);
}

void SceneGame::update(double t)
{
	switch (_currentState) {
	case PLAYING:
		if (_forceRegistry) {
			_forceRegistry->updateForces(t);
		}
		updateGameState(t);
		break;

	case VICTORY_SEQUENCE:
		_victorySequenceTimer -= t;
		if (_victorySequenceTimer <= 0.0f) {
			_currentState = SHOW_VICTORY_MENU;
		}
		break;

	case SHOW_VICTORY_MENU:
		updateVictoryMenuState(t);
		break;

	case GAME_OVER:
		break;
	}

	Scene::update(t);
}
void SceneGame::updateVictoryMenuState(double t)
{
	// Si es el último nivel, manejar timer automático
	const bool isLastLevel = (_currentLevel >= _levels.size() - 1);

	if (isLastLevel && _victorySequenceTimer > 0) {
		_victorySequenceTimer -= t;

		// Cuando el timer llega a 0, volver al menú
		if (_victorySequenceTimer <= 0.0f) {
			if (sceneManager) {
				sceneManager->changeScene(7);
			}
		}
	}
}
void SceneGame::enter()
{
	current_projection_mode = PROJ_ORTHOGRAPHIC;
	Scene::enter();
	setupCamera();
	_shooter->create_renderItem();
	_targetFish->create_renderItem();
	_currentState = PLAYING;
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
	_targetFish = new Fish(getRelativePosition(_posTargetFish.x, _posTargetFish.y), { 0.6f,0.45f,0.85f,1.0f }, 0.6f);

	createGround(100.0f, 100.0f, 2.0f);
	createTarget(getRelativePosition(_posTarget.x, _posTarget.y), 2.0f);
	
	createPlatforms(lvl.platforms);
	createForceZones(lvl.forceZones);
	createAlgaeField();

	_fireworkSystem1 = new FireworkParticleSystem(getRelativePosition(0.2, 0.0));
	_fireworkSystem2 = new FireworkParticleSystem(getRelativePosition(0.8, 0.0));
	addParticleSystem(_fireworkSystem1);
	addParticleSystem(_fireworkSystem2);

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
}

SolidProjectil* SceneGame::getProjectileFromActor(physx::PxActor* const  actor)
{
	if (!actor) return nullptr;

	for (auto* solid : _solids) {
		if (!solid) continue;

		if (solid->getType() == SolidEntity::PROJECTILE) {

			SolidProjectil* proj = static_cast<SolidProjectil*>(solid);
			if (!proj) continue;

			if (!proj->is_alive()) continue;
			if (proj->getActor() == nullptr) continue;

			if (proj->getActor() == actor)
				return proj;
		}
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
	bool isLastLevel = (_currentLevel >= _levels.size() - 1);

	_currentState = VICTORY_SEQUENCE;
	_victorySequenceTimer = _victorySequenceDuration;

	if (_forceRegistry) {
		_forceRegistry->clearSolid(proj);
	}
	_fireSystem = new FireParticleSystem(target->getPos(), 5.0f, Vector4(0.1f, 0.9f, 0.8f, 1.0f), Vector4(0.0f, 0.2f, 0.4f, 0.0f), 1.5f);
	_particleSystems.push_back(_fireSystem);
	_target->kill();
	_target = nullptr;
	if (_fireworkSystem1)_fireworkSystem1->createFirework();
	if (_fireworkSystem2)_fireworkSystem2->createFirework();
}

void SceneGame::initLevels()
{
	_levels.clear();

	//NIVEL1
	_levels.push_back({
		{0.05f,0.2f},  //shooter pos
		{0.8f,0.7f},  // target Pos
		{0.79f,0.7f},  //TargetFish pos
		// Plataformas
		{
			{getRelativePosition(0.2f,0.1f), Vector3(4,0.7,3), Vector4(0.65f,0.60f,0.55f,1.0f)},
			{getRelativePosition(0.4f,0.3f), Vector3(0.7,4,2.5),Vector4(0.55f,0.65f,0.60f,1.0f)},
			{getRelativePosition(0.5f,0.6f), Vector3(0.7,4,2.5),Vector4(0.60f,0.65f,0.70f,1.0f)},
			{getRelativePosition(0.8f,0.4f), Vector3(5,0.7,2.5),Vector4(0.70f,0.60f,0.65f,1.0f)}
		},
		// Zonas de fuerza
		{
			{getRelativePosition(0.0f,0.7f), Vector3(2.2f,2.5f,2.5f), ForceType::WIND},
			{getRelativePosition(0.6f,1.0f), Vector3(2.5f,2.5f,2.5f), ForceType::WHIRLWIND}
		},
		// Fuerzas ambientales 
		{
			// Viento a la izquierda, arriba, hacia la derecha
			{ getRelativePosition(0.05f, 0.7f), Vector3(1,0,0), 5.0f, ForceType::WIND },
				// Torbellino cerca del suelo, gira a la derecha y sube un poco
			{getRelativePosition(0.6f, 0.4f), Vector3(0.5f,1.0f,0.5f), 4.0f, ForceType::WHIRLWIND}
		}
		});

	//NIVEL2
	_levels.push_back({
		{0.05f,0.9f},  //shooter pos
		{0.1f,0.1f},  // target Pos
		{0.08f,0.1f},  //TargetFish pos
		// Plataformas
		{
			{getRelativePosition(0.001f,0.3f), Vector3(4,0.7,3), Vector4(0.65f,0.60f,0.55f,1.0f)},
			{getRelativePosition(0.3f,0.6f), Vector3(4,0.7,2.5), Vector4(0.55f,0.65f,0.60f,1.0f)},
			{getRelativePosition(0.6f,0.6f), Vector3(0.8,5,2.5), Vector4(0.60f,0.65f,0.70f,1.0f)},
			{getRelativePosition(0.8f,0.7f), Vector3(4,0.8,2.5), Vector4(0.70f,0.60f,0.65f,1.0f)}
		},
		// Zonas de fuerza
		{
			{getRelativePosition(0.0f,0.5f), Vector3(2.0f,2.5f,2.5f), ForceType::WIND},
			{getRelativePosition(0.9f,0.6f), Vector3(2.5f,2.5f,2.5f), ForceType::WHIRLWIND}
		},
		// Fuerzas ambientales 
		{
			{ getRelativePosition(0.6f, 0.1f), Vector3(-1,0,0), 6.0f, ForceType::WIND },
			// Torbellino cerca del suelo, gira a la derecha y sube un poco
			{getRelativePosition(0.55f, 0.45f), Vector3(0.5f,1.0f,0.5f), 4.0f, ForceType::WHIRLWIND}
	}
		});
}

void SceneGame::clearScene()
{
	if (collisionCallback)
		collisionCallback->clearForceZones();

	_windActive = false;
	_whirlwindActive = false;

	removePacticleSystem(_fireSystem);
	removePacticleSystem(_fireworkSystem1);
	removePacticleSystem(_fireworkSystem2);
	_fireSystem = nullptr;
	_fireworkSystem1 = nullptr;
	_fireworkSystem2 = nullptr;
	for (auto a : _algaeSystem) {
		removePacticleSystem(a);
	}
	_algaeSystem.clear();

	removePacticleSystem(_windParticleSystem);
	removePacticleSystem(_whirlwindParticleSystem);
	_windParticleSystem = nullptr;
	_whirlwindParticleSystem = nullptr;

	if (_forceRegistry) _forceRegistry->clear();

	Scene::clean();

	_shooter = nullptr;
	_targetFish = nullptr;
	_target = nullptr;

}

void SceneGame::restartLevel()
{
	_projectilesRemaining = _maxProjectiles;

	_shooter->deregisterRenderItem();
	_targetFish->deregisterRenderItem();

	Scene::exit();

	loadLevel(_currentLevel);

	Scene::enter();
	if (_shooter) _shooter->create_renderItem();
	if (_targetFish) _targetFish->create_renderItem();
}

void SceneGame::nextLevel()
{
	_projectilesRemaining = _maxProjectiles;

	_currentLevel++;

	if (_currentLevel >= _levels.size()) {
		_currentState = SHOW_VICTORY_MENU; 
		_victorySequenceTimer = 5.0f;
		return;
	}
	if (_shooter) _shooter->deregisterRenderItem();
	if (_targetFish) _targetFish->deregisterRenderItem();
	Scene::exit();

	loadLevel(_currentLevel);

	if (_shooter) _shooter->create_renderItem();
	if (_targetFish) _targetFish->create_renderItem();
	Scene::enter();

}

#pragma endregion

#pragma region Input
void SceneGame::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
	case ' ':
	{
		if (_currentState == PLAYING) {
			shoot();
		}
		break;
	}
	case '1':
		if (_currentState == PLAYING) {
			toggleForce(WIND);
		}
		break;

	case '2':
		if (_currentState == PLAYING) {
			toggleForce(WHIRLWIND);
		}
		break;
	case 'R':
		if (_currentState == SHOW_VICTORY_MENU || _currentState == GAME_OVER) {
			restartLevel();
			_currentState = PLAYING;
		}
		break;

	case 'N':
		if (_currentState == SHOW_VICTORY_MENU) 
		{
			bool isLastLevel = (_currentLevel >= _levels.size() - 1);
			if (isLastLevel) 
			{
				if (sceneManager)
				{
					sceneManager->changeScene(7);
				}
			}
			else 
			{
				nextLevel();
				_currentState = PLAYING;
			}
			
		}
		break;
	default:
	{ 
		break;
	}
		
		
	}
}

void SceneGame::handleSpecialInput(int key)
{
	if (_currentState != PLAYING) return;
	switch (key) {
	case GLUT_KEY_LEFT:
		updateShootAngle(-0.1f);
		break;
	case GLUT_KEY_RIGHT:
		updateShootAngle(0.1f);
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
	if (_projectilesRemaining <= 0 || !_shooter || _currentState != PLAYING)
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
	_projectilesRemaining--;

}

void SceneGame::updateShootAngle(float delta)
{
	if (!_shooter) return;

	float newAngle = _shooter->getAngle() + delta;

	if (newAngle > MAX_ANGLE) newAngle = MAX_ANGLE;
	if (newAngle < MIN_ANGLE) newAngle = MIN_ANGLE;

	_shooter->setAngle(newAngle);
}
#pragma endregion

#pragma region GameInfo
void SceneGame::updateGameState(double t)
{
	if (_currentState != PLAYING) return;

	if (_projectilesRemaining <= 0 && _currentState == PLAYING) {
		_currentState = GAME_OVER;
		return;
	}
	std::stringstream ss;
	ss << "FLECHAS Girar | SPACE Disparar | 1 Viento | 2 Torbellino  "
		<< " | Proyectiles " << _projectilesRemaining << "/" << _maxProjectiles
		<< " | Nivel " << (_currentLevel + 1) << "/" << _levels.size();
	_text = ss.str();
}

#pragma endregion

#pragma region Interfaz
void SceneGame::render_interface()
{
	int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	int screenHeight = glutGet(GLUT_WINDOW_HEIGHT);

	switch (_currentState) {
	case SHOW_VICTORY_MENU:
		drawFullscreenOverlay(_blackColor);
		drawVictoryScreen();
		break;

	case GAME_OVER:
		drawFullscreenOverlay(_blackColor);
		drawGameOverScreen();
		break;
	case PLAYING:
	default:
		break;
	}
}

void SceneGame::drawVictoryScreen()
{
	int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	int screenHeight = glutGet(GLUT_WINDOW_HEIGHT);

	bool isLastLevel = (_currentLevel >= _levels.size() - 1);

	if (isLastLevel) {
		float goldColor[4] = { 1.0f, 0.84f, 0.0f, 1.0f };  // Dorado

		drawText("JUEGO COMPLETADO",
			screenWidth / 2.0f, screenHeight * 0.7f,
			80, true, true, false, goldColor);

		drawText("Has rescatado a todos tus amigos",
			screenWidth / 2.0f, screenHeight * 0.55f,
			35, true, true, false, _whiteColor);

		if (_victorySequenceTimer > 0) {
			std::string timerText = "Volviendo al menú en " +
				std::to_string(static_cast<int>(ceil(_victorySequenceTimer))) + " segundos";
			drawText(timerText,
				screenWidth / 2.0f, screenHeight * 0.4f,
				30, true, true, false, _whiteColor);
		}

		drawText("Presiona ESC para salir",
			screenWidth / 2.0f, screenHeight * 0.3f,
			25, true, true, false, _whiteColor);

	}
	else {
		drawText("VICTORIA",
			screenWidth / 2.0f, screenHeight * 0.7f,
			80, true, true, false, _victoryColor);

		drawText("Presiona R para reiniciar nivel",
			screenWidth / 2.0f, screenHeight * 0.5f,
			30, true, true, false, _whiteColor);

		drawText("Presiona N para siguiente nivel",
			screenWidth / 2.0f, screenHeight * 0.4f,
			30, true, true, false, _whiteColor);
	}
}
void SceneGame::drawGameOverScreen()
{
	int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	int screenHeight = glutGet(GLUT_WINDOW_HEIGHT);

	drawText("GAME OVER",
		screenWidth / 2.0f, screenHeight * 0.7f,
		80, true, true, true, _gameOverColor, _whiteColor);

	drawText("Presiona R para reintentar",
		screenWidth / 2.0f, screenHeight * 0.5f,
		30, true, false, false, _whiteColor, nullptr);
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
	}
	if (_targetFish) {
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
		Vector4(1.0f, 0.3f, 0.8f, 0.3f)
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
	std::vector<float> relX = { 0.1f, 0.4f,0.45f, 0.6f, 0.75f, 0.8, 0.95 };


	for (auto& rx : relX) {
		AlgaeParticleSystem* a = new AlgaeParticleSystem(getRelativePosition(rx, 0.01f), 50);
		_particleSystems.push_back(a);
	}
}
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

	for (auto& s : _solids) {
		if (s->getType() == SolidEntity::PROJECTILE) {
			SolidProjectil* proj = static_cast<SolidProjectil*>(s);
			if (proj->is_alive()) {
				if (active) {
					_forceRegistry->addRegistry(proj, forceGenerator);
				}
				else {
					_forceRegistry->remove(proj, forceGenerator);
				}
		}
		}
		
	}
}
#pragma endregion