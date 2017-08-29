#ifndef SCENEWIN_H
#define SCENEWIN_H

#include "SceneBase.h"
#include "Button.h"

class SceneGameOver : public SceneBase
{
public:
	SceneGameOver();
	~SceneGameOver();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	float m_worldWidth;
	float m_worldHeight;
};

#endif //!SCENEWIN_H