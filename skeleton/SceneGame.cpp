#include "SceneGame.h"
#include "GravityGenerator.h"
#include "WindGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionGenerator.h"
#include "RenderUtils.hpp"
#include <iostream>
#include <sstream>

SceneGame::SceneGame()
	:_target(nullptr), _currentProjectile(nullptr),
	_windGenerator(nullptr), _gravityGenerator(nullptr), _explosionGenerator(nullptr), 
	_projectilePower(20.0f),_projectilesRemaining(8), _maxProjectiles(80),
	_windActive(true), _windStrength(200.0f),_gameWon(false),
	_worldWidth(30.0f), _worldHeight(25.0f)
{
}

SceneGame::~SceneGame()
{
	delete _windGenerator;
	delete _gravityGenerator;
	delete _explosionGenerator;
	delete _shooter;
}

void SceneGame::init()
{
	_text = "FLECHAS: Girar/Disparar, ESPACIO: Disparar, V: Viento, R: Reset";
	clearScene();

	updateViewportFromScreen();

	createGameObjects();
	setupForces();

	_gameWon = false;
	_projectilesRemaining = _maxProjectiles;
}

void SceneGame::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
	case ' ': // Disparar proyectil
	{
		shoot();
		break;
	}
	case 'V': {
		_windActive = !_windActive;

		if (_currentProjectile) {
			if (_windActive)
				_forceRegistry->addRegistry(_currentProjectile, _windGenerator);
			else
				_forceRegistry->remove(_currentProjectile, _windGenerator);
		}
		break;
	}	
	case 'R': // Resetear juego
		init();
		break;

	case 'E':
		break;

	default:
		break;
	}

	showGameInfo();
}

void SceneGame::handleSpecialInput(int key)
{
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

void SceneGame::update(double t)
{
	updateViewportFromScreen();
	Scene::update(t);
	updateGameState(t);
	checkProjectileState();
	checkGameOver();
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
	_particles.clear();
	if (_forceRegistry) _forceRegistry->clear();
	_currentProjectile = nullptr;

	delete _shooter;
	_shooter = nullptr;
}

void SceneGame::createGameObjects()
{
	_shooter = new Shooter(getRelativePosition(0.05f, 0.05f), Vector4(1, 0, 0, 1));
	
	_target = new Particle(getRelativePosition(0.8f, 0.5f), Vector3(0), Vector3(0),
		Vector4(1.0, 0.0, 0.0, 1.0), 0, 1000);
	_target->setTam(2.0);
	_particles.push_back(_target);
}

void SceneGame::setupForces()
{
	//_gravityGenerator = new GravityGenerator(Vector3(0, -9.8, 0));

	Vector3 windDir = Vector3(-1, 0, 0);
	//_windGenerator = new WindGenerator(Vector3(0, 5, 0), 5, windDir * _windStrength, 0.1);

}

void SceneGame::setupCamera()
{
	if (GetCamera()) {
		GetCamera()->setEye({ 0.0f, 0.0f, 25.0f });
		GetCamera()->setDir({ 0.0f, 0.0f, -1.0f });
	}
}

void SceneGame::shoot()
{
	if (_projectilesRemaining <= 0 || _currentProjectile != nullptr || !_shooter)
		return;

	Vector3 dir = _shooter->getShootDirection();
	dir.normalize();
	const Vector3 ini_pos = _shooter->getPosition();
	_currentProjectile = new Projectil(ini_pos, _projectilePower * dir, ProjectilType::GAME, Vector4(0.0, 0.0, 1.0, 1.0));
	_currentProjectile->setTam(0.5);
	addEntityWithRenderItem(_currentProjectile);

	if (_gravityGenerator)
		_forceRegistry->addRegistry(_currentProjectile, _gravityGenerator);
	if (_windGenerator && _windActive)
		_forceRegistry->addRegistry(_currentProjectile, _windGenerator);

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
	if (!_currentProjectile) return;

	Vector3 projPos = _currentProjectile->getPos();
	Vector3 targetPos = _target->getPos();

	if ((projPos - targetPos).magnitude() < 2.5f) {
		_gameWon = true;
		_text = "¡OBJETIVO DESTRUIDO! Presiona R para reiniciar";
	}

	if (projPos.y < -5 || abs(projPos.x) > 50 || abs(projPos.z) > 50) {
		_currentProjectile = nullptr;
	}
}

void SceneGame::checkGameOver()
{
	if (_projectilesRemaining <= 0 && !_currentProjectile && !_gameWon) {
		_text = "GAME OVER： Sin proyectiles. Presiona R para reiniciar";
	}
}

void SceneGame::showGameInfo()
{
	const float angleDeg = _shooter ? (_shooter->getAngle() * 180 / 3.14159f) : 0.0f;

	std::cout << "\nAngulo: " << angleDeg
		<< " | Potencia: " << _projectilePower
		<< " | Proyectiles: " << _projectilesRemaining << "/" << _maxProjectiles
		<< " | Viento: " << (_windActive ? "ON" : "OFF") << std::endl;
}

void SceneGame::updateGameState(double t)
{
	std::stringstream ss;
	ss << "FLECHAS: Girar/Disparar | Potencia: " << _projectilePower
		<< " | Proyectiles: " << _projectilesRemaining << "/" << _maxProjectiles
		<< " | V: Viento (" << (_windActive ? "ON" : "OFF") << ") | R: Reset";

	_text = ss.str();
}

Vector3 SceneGame::getRelativePosition(float relX, float relY, float z) const
{
	float worldX = (relX - 0.5f) * _worldWidth;
	float worldY = (relY - 0.5f) * _worldHeight;

	return Vector3(worldX, worldY, z);
}

void SceneGame::updateViewportFromScreen()
{
	int screenWidth= glutGet(GLUT_WINDOW_WIDTH);
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
