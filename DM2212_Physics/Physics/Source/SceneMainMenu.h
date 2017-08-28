#ifndef SCENE_MAINMENU_H
#define SCENE_MAINMENU_H

#include "SceneBase.h"
#include "Button.h"

class SceneMainMenu : public SceneBase
{

public:
	SceneMainMenu();
	~SceneMainMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText();//render text with background
	void RenderTest(Vector3 mousepos);

protected:
	float m_worldWidth;
	float m_worldHeight;
	bool inMapSelect;
	bool inControls;
	static const int numButtons = 2;
	Button* ButtArray[numButtons];
	
};

#endif