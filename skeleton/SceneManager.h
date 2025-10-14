#pragma once
#include <vector>
#include <memory>
#include <string>

class Scene;
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void update(double t);
	void changeScene(int sceneIndex);
	void nextScene();
	void handleInput(unsigned char key);
	std::string getDisplayText() const;

private:
	std::vector<std::unique_ptr<Scene>> _scenes;
	int _current_scene;
	void initScenes();
};

