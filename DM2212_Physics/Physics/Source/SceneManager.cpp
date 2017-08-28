#include "SceneManager.h"
#include "Application.h"
#include "SceneCollision.h"
#include "SceneEditor.h"
#include "SceneMainMenu.h"

SceneManager* SceneManager::scenemanager;
int SceneManager::prevscene = DEFAULT;
int SceneManager::currscene = DEFAULT;

SceneManager::~SceneManager()
{
	scenes.erase(scenes.begin(), scenes.end());
}

SceneManager * SceneManager::getInstance()
{
	if (!scenemanager)
	{
		scenemanager = new SceneManager;
	}
	return scenemanager;
}

void SceneManager::Init(int key)
{
	scenes[key]->Init();
}

void SceneManager::Update()
{
	scenemanager->scenes[currscene]->Update(Application::m_timer.getElapsedTime());
	if (prevscene != currscene)
	{
		//Loading...
		/*scenemanager->ChangeScene(3);
		scenemanager->scenes[3]->Render();*/
		//Swap buffers
		glfwSwapBuffers(Application::m_window);
		scenemanager->ChangeScene(currscene);
	}
	scenemanager->scenes[currscene]->Render();
	//Swap buffers
	glfwSwapBuffers(Application::m_window);
	glfwPollEvents();
	Application::m_timer.waitUntil(Application::frameTime);
}

void SceneManager::ChangeScene(int key)
{
	scenemanager->Exit(scenemanager->prevscene);
	scenemanager->Init(key);
	scenemanager->prevscene = key;
}

void SceneManager::Exit(int key)
{
	scenes[key]->Exit();
}

SceneManager::SceneManager()
{
	prevscene = DEFAULT;
	scenes[1] = new SceneMainMenu;
	scenes[2] = new SceneCollision;
	scenes[3] = new SceneEditor;
}
