#include "SceneGame.h"
#include "GravityGenerator.h"
#include "WindGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionGenerator.h"

SceneGame::SceneGame()
	:_canon(nullptr), _target(nullptr), _currentProjectile(nullptr),
	_windGenerator(nullptr), _gravityGenerator(nullptr),_explosionGenerator(nullptr),
	_canonAngle(0.0f), _projectilePower(20.0f),
	_projectilesRemaining(8), _maxProjectiles(8),
	_windActive(true), _windStrength(8.0f),
	_gameWon(false), _gameTime(0.0f)
{}

SceneGame::~SceneGame()
{
	delete _windGenerator;
	delete _gravityGenerator;
	delete _explosionGenerator;
}

void SceneGame::init()
{
	_text = "A/D: Girar, W/S: Potencia, ESPACIO: Disparar, V: Viento, R: Reset";
	
	setupCamera();

	createCanon();
	createTarget();
	createPlatforms();
	createWindZone();

	showGameInfo();

	_gravityGenerator = new GravityGenerator(Vector3(0, -9.8, 0));
	addGlobalForce(_gravityGenerator);
}

void SceneGame::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
		case '1':
		{
			Particle* p1 = new Particle(
				Vector3(5, 10, 0), Vector3(0, 0, 0),
				Vector3(0, 0, 0), Vector4(1, 0, 0, 1)  // Rojo
			);
			p1->setMass(1);
			_forceRegistry->addRegistry(p1, _gravityGenerator);
			addEntityWithRenderItem(p1);
			_particles.push_back(p1);

			Particle* p2 = new Particle(
				Vector3(0, 10, 0), Vector3(0, 0, 0),
				Vector3(0, -9.8, 0), Vector4(0, 1, 0, 1)  // Verde
			);
			addEntityWithRenderItem(p2);

			break;
		}
		case '2':
		{
			WindGenerator* wind = new WindGenerator(Vector3(0, 0, 0), 50, Vector3(0, 100, 0), 0.15);
			Particle* p1 = new Particle(
				Vector3(5, 10, 0), Vector3(0, 0, 0),
				Vector3(0, 0, 0), Vector4(1, 0, 0, 1)
			);
			p1->setMass(1);
			_forceRegistry->addRegistry(p1, wind);
			addEntityWithRenderItem(p1);
			break;
		}
		case '3':  // Tecla 3: Crear m迆ltiples part赤culas para probar explosi車n
		{
			for (int i = 0; i < 15; ++i) {
				double x = (rand() % 300 - 150) / 10.0;  // -15 a 15
				double y = (rand() % 100 + 50) / 10.0;   // 5 a 15
				double z = (rand() % 300 - 150) / 10.0;  // -15 a 15

				double mass = 1.0; // 1.0 a 21.0

				Particle* p = new Particle(
					Vector3(x, y, z),
					Vector3(0, 0, 0),
					Vector3(0, 0, 0),
					Vector4(0.8, 0.6, 0.2, 1.0),  // Color naranja
					mass
				);

				//_forceRegistry->addRegistry(p, _gravityGenerator);
				_forceRegistry->addRegistry(p, _explosionGenerator);
				addEntityWithRenderItem(p);
			}
			break;
		}
		case 'E':  // Tecla E: Activar explosi車n
		{
			if (_explosionGenerator != nullptr) {
				_explosionGenerator->activate();
			}
			break;
		}
		case 'R':  // Tecla R: Resetear explosi車n (permitir reactivaci車n)
		{
			if (_explosionGenerator != nullptr) {
				_explosionGenerator->deactivate();
			}
			break;
		}
		default: {
			break;
		}
	}
}

void SceneGame::update(double t)
{
}

void SceneGame::enter()
{
}

void SceneGame::exit()
{
	if (GetCamera())GetCamera()->resetCamera();
}

void SceneGame::setupCamera()
{
	if (GetCamera()) {
		GetCamera()->setEye({ 0.0f, 10.0f, 25.0f });
		GetCamera()->setDir({ 0.0f, -0.3f, -1.0f });
	}
}

void SceneGame::createCanon()
{
}

void SceneGame::createTarget()
{
}

void SceneGame::createPlatforms()
{
}

void SceneGame::createWindZone()
{
}

void SceneGame::shootProjectile()
{
}

void SceneGame::updateCanonAngle()
{
}

void SceneGame::updateProjectilePower()
{
}

void SceneGame::checkWinCondition()
{
}

void SceneGame::resetProjectile()
{
}

void SceneGame::showGameInfo()
{
}
