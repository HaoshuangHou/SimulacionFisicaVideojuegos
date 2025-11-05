#include "Scene4.h"
#include "GravityGenerator.h"
#include "WindGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionGenerator.h"

void Scene4::init()
{
	_text = "ESCENA 4: FUERZAS, SIGUIENTE ESCENA(Q), gravedad(1), viento(2), torbellino(3), explosion(4)";

	_gravityGenerator = new GravityGenerator(Vector3(0, -9.8, 0));

	_explosionGenerator = new ExplosionGenerator(Vector3(0, 10, 0), 15.0, 5000.0, 10.0);

	_windGenerator = new WindGenerator(Vector3(0, 10, 0), 15, Vector3(0, 100, 0), 0.15);

	_whirlwindGenerator = new WhirlwindGenerator(Vector3(0, 10, 0), 15, 2.0, 0.08, 0.02);
}

void Scene4::enter()
{
	Scene::enter();
}

void Scene4::exit()
{
	Scene::exit();
	_explosionGenerator->setVisible(false);
	_windGenerator->setVisible(false);
	_whirlwindGenerator->setVisible(false);
}

void Scene4::handleInput(unsigned char key)
{
	switch (toupper(key))
	{
		case '1':
		{
			_explosionGenerator->setVisible(false);
			_windGenerator->setVisible(false);
			_whirlwindGenerator->setVisible(false);
			Particle* p1 = new Particle(
				Vector3(5, 20, 0), Vector3(0, 0, 0),
				Vector3(0, 0, 0), Vector4(1, 0, 0, 1),  // Rojo
				1.0f
			);
			_forceRegistry->addRegistry(p1, _gravityGenerator);
			addEntityWithRenderItem(p1);
			break;
		}
		case '2':
		{
			_explosionGenerator->setVisible(false);
			_windGenerator->setVisible(true);
			_whirlwindGenerator->setVisible(false);
			Particle* p = new Particle(
				Vector3(0, 0, 0), Vector3(0, 0, 0),
				Vector3(0, 0, 0), Vector4(1, 0, 1, 1),
				1.0f
			);
			_forceRegistry->addRegistry(p, _windGenerator);
			_forceRegistry->addRegistry(p, _gravityGenerator);
			addEntityWithRenderItem(p);

			break;
		}
		case '3':
		{
			_explosionGenerator->setVisible(false);
			_windGenerator->setVisible(false);
			_whirlwindGenerator->setVisible(true);
			for (int i = 0; i < 5; ++i) {
				double angle = i * (360.0 / 5) * (3.14159 / 180.0);
				double radius = 4.0;    
				double x = cos(angle) * radius;
				double z = sin(angle) * radius;

				Particle* p = new Particle(
					Vector3(x, 0, z), 
					Vector3(0, 0, 0),
					Vector3(0, 0, 0),
					Vector4(0, 1, 1, 1),
					1.0f
				);
				_forceRegistry->addRegistry(p, _whirlwindGenerator);
				_forceRegistry->addRegistry(p, _gravityGenerator);
				addEntityWithRenderItem(p);
			}
			break;
		}
		case '4':
		{
			_explosionGenerator->setVisible(true);
			_windGenerator->setVisible(false);
			_whirlwindGenerator->setVisible(false);

			for (int i = 0; i < 10; ++i) {
				double angle = i * (360.0 / 10) * (3.14159 / 180.0);
				double radius = 4.0;
				double x = cos(angle) * radius;
				double z = sin(angle) * radius;

				double mass = 1.0f + i;

				Particle* p = new Particle(
					Vector3(x, 10, z),
					Vector3(0, 0, 0),
					Vector3(0, 0, 0),
					Vector4(1, 1, 0, 1),
					mass
				);
				_forceRegistry->addRegistry(p, _explosionGenerator);
				addEntityWithRenderItem(p);
			}
			_explosionGenerator->active(true);
			break;
		}
		default: {
			break;
		}
	}
}