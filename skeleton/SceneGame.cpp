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

#include <iostream>
#include <sstream>

SceneGame::SceneGame()
	:_target(nullptr),
	_windGenerator(nullptr), _gravityGenerator(nullptr), _explosionGenerator(nullptr), 
	_fireSystem(nullptr), _fireworkSystem(nullptr), _fog(nullptr),
	_windParticleSystem(nullptr), _whirlwindParticleSystem(nullptr),
	_projectilePower(20.0f),_projectilesRemaining(8), _maxProjectiles(8),
	_windActive(false), _whirlwindActive(false),_gameWon(false),_gameOver(false),
	_worldWidth(30.0f), _worldHeight(25.0f), _explosionTimer(0)
{}

SceneGame::~SceneGame()
{
	_activeProjectiles.clear();
	delete _shooter;
	_shooter = nullptr;
}

#pragma region Scene
void SceneGame::init()
{
	_text = "FLECHAS Girar / Potencia | SPACE Disparar | 1 Viento | 2 Torbellino ";
	clearScene();

	updateViewportFromScreen();

	createGameObjects();
	setupForces();

	_gameWon = false;
	_gameOver = false;
	_projectilesRemaining = _maxProjectiles;
	_windActive = false;
	_whirlwindActive = false;
}

void SceneGame::update(double t)
{
	updateGameState(t);
	updateViewportFromScreen();
	if (_gameWon) {
		if (_explosionTimer > 0.0) {
			_explosionTimer -= t;
		}
		else if (_explosionGenerator) {
			_explosionGenerator->active(true);
		}
	}
	else if (!_gameOver && !_gameWon) {
		checkProjectileState();
	}

	Scene::update(t);
}

void SceneGame::enter()
{
	current_projection_mode = PROJ_ORTHOGRAPHIC;
	Scene::enter();
	setupCamera();
	_shooter->create_renderItem();
}

void SceneGame::exit()
{
	current_projection_mode = PROJ_PERSPECTIVE;
	Scene::exit();
	if (GetCamera()) GetCamera()->resetCamera();
	_shooter->deregisterRenderItem();
}

void SceneGame::clearScene()
{
	_activeProjectiles.clear();
	_particles.clear();

	if (_forceRegistry) _forceRegistry->clear();

	removePacticleSystem(_fireSystem);
	removePacticleSystem(_fog);
	removePacticleSystem(_fireworkSystem);

	if(_windParticleSystem)_windParticleSystem->setVisible(false);
	if(_whirlwindParticleSystem)_whirlwindParticleSystem->setVisible(false);

	if (_explosionGenerator) {
		_explosionGenerator->active(false);
	}
}
void SceneGame::setupCamera()
{
	if (GetCamera()) {
		GetCamera()->setEye({ 0.0f, 0.0f, 25.0f });
		GetCamera()->setDir({ 0.0f, 0.0f, -1.0f });
	}
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
	case 'R': 
		init();
		break;

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
	if (_projectilesRemaining <= 0 || !_shooter || _gameWon)
		return;

	Vector3 dir = _shooter->getShootDirection();
	dir.normalize();
	const Vector3 ini_pos = _shooter->getPosition();

	Projectil* newProjectile = new Projectil(ini_pos, _projectilePower * dir, ProjectilType::GAME, Vector4(0.0, 0.0, 1.0, 1.0));
	newProjectile->setTam(0.5);
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
	_activeProjectiles.push_back(newProjectile);
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

void SceneGame::updateProjectilePower(float delta)
{
	_projectilePower += delta;

	if (_projectilePower > MAX_POWER) _projectilePower = MAX_POWER;
	if (_projectilePower < MIN_POWER) _projectilePower = MIN_POWER;
}

void SceneGame::checkProjectileState()
{
	for (auto& projectile : _activeProjectiles) {
		if (!projectile || !projectile->is_alive()) {
			continue;
		}

		const Vector3 projPos = projectile->getPos();
		const Vector3 targetPos = _target->getPos();

		if ((projPos - targetPos).magnitude() < 1.5f) {
			_gameWon = true;
			projectile->setAlive(false);

			_windParticleSystem->setVisible(false);
			_whirlwindParticleSystem->setVisible(false);

			_fireSystem = new FireParticleSystem(_victoryPos, 5.0f);
			addParticleSystem(_fireSystem);
			_fireSystem->addForce(_explosionGenerator);

			_fireworkSystem = new FireworkParticleSystem(getRelativePosition(0.5f, 0.0f));
			_fireworkSystem->createFirework();
			addParticleSystem(_fireworkSystem);

			_explosionTimer = 3.0f;
			_explosionGenerator->active(false);
		}
	}

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
			_windParticleSystem->setVisible(false);
			_whirlwindParticleSystem->setVisible(false);
			_fog = new FogParticleSystem(_victoryPos, 100.0f);
			addParticleSystem(_fog);
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
Vector3 SceneGame::getRelativePosition(float relX, float relY, float z) const
{
	float worldX = (relX - 0.5f) * _worldWidth;
	float worldY = (relY - 0.5f) * _worldHeight;

	return Vector3(worldX, worldY, z);
}

void SceneGame::updateViewportFromScreen()
{
	int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	int	screenHeight = glutGet(GLUT_WINDOW_HEIGHT);
	float screenAspect = (float)screenWidth / (float)screenHeight;

	float baseWorldHeight = 25.0f;
	float baseWorldWidth = 30.0f;
	float baseAspect = baseWorldWidth / baseWorldHeight;

	float oldWorldWidth = _worldWidth;
	float oldWorldHeight = _worldHeight;

	// Calcular nuevas dimensiones
	if (screenAspect > baseAspect) {
		_worldHeight = baseWorldHeight;
		_worldWidth = baseWorldHeight * screenAspect;
	}
	else {
		_worldWidth = baseWorldWidth;
		_worldHeight = baseWorldWidth / screenAspect;
	}

	// Si las dimensiones cambiaron, reposicionar objetos
	if (oldWorldWidth != _worldWidth || oldWorldHeight != _worldHeight) {
		repositionObjects();
	}
}

void SceneGame::repositionObjects()
{
	if (_shooter) {
		_shooter->setPosition(getRelativePosition(0.05f, 0.05f));
	}

	// Reposicionar el objetivo
	if (_target) {
		_target->setPosition(getRelativePosition(0.8f, 0.5f));
	}
}
#pragma endregion

#pragma region GameObjects
void SceneGame::createGameObjects()
{
	if (_gameOver || _gameWon) return;
	
	_shooter = new Shooter(getRelativePosition(0.05f, 0.05f), Vector4(1, 0, 0, 1));

	_target = new Particle(getRelativePosition(0.8f, 0.6f), Vector3(0), Vector3(0),
		Vector4(1.0, 0.0, 0.0, 1.0), 0, 1000);
	_target->setTam(1.0);
	_particles.push_back(_target);

	_victoryPos = _target->getPos();
}
#pragma endregion

#pragma region Forces
void SceneGame::setupForces()
{
	_gravityGenerator = new GravityGenerator(Vector3(0, -9.8, 0));

	const Vector3 windPos = getRelativePosition(0.3f, 0.2f);
	const Vector3 windDir = Vector3(0, 1, 0);
	_windGenerator = new WindGenerator(windPos, 5, windDir *50, 0.3);

	const Vector3 whirlwindPos = getRelativePosition(0.4f, 0.6f);
	_whirlwindGenerator = new WhirlwindGenerator(whirlwindPos, 4.0f, 1.8f, 0.2f, 0.05f, true);

	_windParticleSystem = new ForceParticleSystem(
		_windGenerator,
		Vector4(0.6f, 0.7f, 0.8f, 0.45f),
		false
	);

	_whirlwindParticleSystem = new ForceParticleSystem(
		_whirlwindGenerator,
		Vector4(0.65f, 0.6f, 0.9f, 0.5f),
		true
	);

	addParticleSystem(_windParticleSystem);
	addParticleSystem(_whirlwindParticleSystem);

	_windParticleSystem->setVisible(false);
	_whirlwindParticleSystem->setVisible(false);

	if (_windParticleSystem) {
		_windParticleSystem->addForce(_windGenerator);
	}
	if (_whirlwindParticleSystem) {
		_whirlwindParticleSystem->addForce(_whirlwindGenerator);
	}

	_explosionGenerator = new ExplosionGenerator(_victoryPos, 5.0f, 100, 5);
}
void SceneGame::toggleForce(ForceType forceType)
{
	switch (forceType) {
	case WIND:
	{
		if (!_windGenerator || !_windParticleSystem) return;
		_windActive = !_windActive;
		_windParticleSystem->setVisible(_windActive);
		applyForceToAllProjectiles(_windGenerator, _windActive);
		break;

	}
	case WHIRLWIND:
	{
		if (!_whirlwindGenerator || !_whirlwindParticleSystem) return;
		_whirlwindActive = !_whirlwindActive;
		_whirlwindParticleSystem->setVisible(_whirlwindActive);
		applyForceToAllProjectiles(_whirlwindGenerator, _whirlwindActive);
		break;
	}
	default:
		break;
	}	
}

void SceneGame::applyForceToAllProjectiles(ForceGenerator* forceGenerator, bool active)
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