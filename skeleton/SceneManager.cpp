#include "SceneManager.h"
#include "Scene1.h"

SceneManager::SceneManager()
	:_current_scene(0)
{
	initScenes();
}
void SceneManager::initScenes()
{
    // Crear las escenas
    _scenes.push_back(std::make_unique<Scene1>());

    // Inicializar la escena actual
    if (!_scenes.empty()) {
        _scenes[_current_scene]->init();
    }
}
void SceneManager::update(double t)
{
    if (_current_scene >= 0 && _current_scene < _scenes.size()) {
        _scenes[_current_scene]->update(t);
    }
}
void SceneManager::changeScene(int sceneIndex)
{
    if (sceneIndex >= 0 && sceneIndex < _scenes.size() && sceneIndex != _current_scene) {
        _current_scene = sceneIndex;
        _scenes[_current_scene]->init();    // Inicializar nueva escena
    }
}
