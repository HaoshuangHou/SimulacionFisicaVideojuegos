#pragma once
#include <vector>
#include <memory>
#include <string>
#include <PxPhysicsAPI.h>

class Scene;
class SceneManager
{
public:
	SceneManager(physx::PxPhysics* physics, physx::PxScene* scene);
	~SceneManager();

	void update(double t);
	void changeScene(int sceneIndex);
	void nextScene();
	void handleInput(unsigned char key);
    void handleSpecialInput(int key);

	Scene* getCurrentScene() const;

	std::string getDisplayText() const;

private:
	physx::PxPhysics* _gPhysics = nullptr;
	physx::PxScene* _gScene = nullptr;
	physx::PxMaterial* _gMaterial = nullptr;

	std::vector<std::unique_ptr<Scene>> _scenes;
	int _current_scene;
	void initScenes();
};

