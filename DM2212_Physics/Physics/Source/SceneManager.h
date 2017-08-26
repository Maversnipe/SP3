#ifndef SCNENEMANAGER_H
#define SCENEMANAGER_H

#define DEFAULT 1

#include <map>
#include "Scene.h"

class SceneManager
{
public:
	~SceneManager();
	static SceneManager *getInstance();

	//Functions
	void Init(int key);
	void Update();
	void ChangeScene(int key);
	void Exit(int key);

	static int currscene;
	static int prevscene;
private:
	static SceneManager* scenemanager;
	std::map<int, Scene*> scenes;
	
	SceneManager();
};

#endif // !SCNENEMANAGER_H
