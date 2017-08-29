#ifndef SCENE_MAINMENU_H
#define SCENE_MAINMENU_H

#include "SceneBase.h"
#include "Button.h"
#include "SoundEngine.h"

class SceneMainMenu : public SceneBase
{

public:
	SceneMainMenu();
	~SceneMainMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();


protected:
	float m_worldWidth;
	float m_worldHeight;
	bool inMapSelect;
	bool inControls;
	static const int numButtons = 4;
	Button* ButtArray[numButtons];
	
};

#endif