#ifndef SCNENEMANAGER_H
#define SCENEMANAGER_H

//Author:Sheng Yang 
//Used changing to different Scenes
//Singleton

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

	//Changing Scenes
	void ChangeScene(int key);
	//Exiting Scenes
	void Exit(int key);

	//current and previous scenes
	static int currscene;
	static int prevscene;

private:
	static SceneManager* scenemanager;

	//Scenesdata
	std::map<int, Scene*> scenes;
	
	SceneManager();
};

#endif // !SCNENEMANAGER_H
