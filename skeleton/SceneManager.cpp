#include "SceneManager.h"
#include "Scene.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Scene5.h"
#include "Scene6.h"
#include "SceneGame.h"

SceneManager::SceneManager(physx::PxPhysics* physics, physx::PxScene* scene)
    :_current_scene(0), _gPhysics(physics), _gScene(scene)
{
	initScenes();
}
SceneManager::~SceneManager()
{

}
void SceneManager::initScenes()
{
    // Crear las escenas
    _scenes.push_back(std::make_unique<Scene1>());
    _scenes.push_back(std::make_unique<Scene2>());
    _scenes.push_back(std::make_unique<Scene3>());
    _scenes.push_back(std::make_unique<Scene4>());
    _scenes.push_back(std::make_unique<Scene5>());
    _scenes.push_back(std::make_unique<Scene6>());
    _scenes.push_back(std::make_unique<SceneGame>());

    // Inicializar
    for (auto& s : _scenes) {
        s->init(_gPhysics, _gScene);
    }
}
void SceneManager::update(double t)
{
    if (_current_scene >= 0 && _current_scene < _scenes.size()) {
        _scenes.at(_current_scene)->update(t);
    }
}
void SceneManager::changeScene(int sceneIndex)
{
    if (sceneIndex >= 0 && sceneIndex < _scenes.size() && sceneIndex != _current_scene) {
        _scenes.at(_current_scene)->exit();
        _current_scene = sceneIndex;
        _scenes.at(_current_scene)->enter();
    }
}

void SceneManager::nextScene()
{
    int next_scene = _current_scene + 1;
    if (next_scene >= _scenes.size())
        next_scene = 0;

    changeScene(next_scene);
}

void SceneManager::handleInput(unsigned char key)
{
    _scenes.at(_current_scene)->handleInput(key);
}

void SceneManager::handleSpecialInput(int key)
{
    if (_current_scene >= 0 && _current_scene < _scenes.size()) {
        _scenes.at(_current_scene)->handleSpecialInput(key);
    }
}

std::string SceneManager::getDisplayText() const
{
    return _scenes.at(_current_scene)->getDisplayText();
}
