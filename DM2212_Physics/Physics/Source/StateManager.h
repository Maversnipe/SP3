#ifndef STATEMANAGER_H
#define STATEMANAGER_H

//Author:Sheng Yang 
//Used In gamme for pausing game and rendering pause screen
//Singleton

#include "Vector3.h"
#include "Camera.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "SoundEngine.h"

#include "Application.h"
#include "SceneBase.h"
#include "Button.h"

enum States
{
	S_PAUSED = 0,
	S_PLAYING,
	S_TOTAL
};

enum PAUSE_BUTTONS
{
	PAUSE_MENU,
	PAUSE_CONTINUE,
	PAUSE_OPTIONS,
	PAUSE_EXIT,
	PAUSE_TOTAL,
};

class StateManager : public SceneBase
{
public:
	~StateManager();
	static StateManager *getInstance();

	//Functions
	void Init();
	void Update(double dt);

	//Change States
	void ChangeState(States state);
	States GetState();

	//Rendering
	virtual void Render();
	void RenderMesh(Mesh *mesh, bool enableLight);

private:
	static StateManager* statemanager;

	Camera camera;

	//Current in game state
	States m_currstate;
	
	//Set limits
	float m_fworldheight;
	float m_fworldwidth;

	bool inControls;
	static const int numButtons = 3;
	Button* Array[numButtons];

	//Buttons
	Mesh* MenuButton[PAUSE_TOTAL];
	Mesh* meshList[NUM_GEOMETRY];

	StateManager();
};

#endif // !STATEMANAGER_H
