#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "SceneBase.h"

class StateManager
{
public:
	enum States
	{
		S_PAUSED=0,
		S_PLAYING,
		S_TOTAL
	};

	~StateManager();
	static StateManager *getInstance();

	//Functions
	void Init();
	void Update();
	void ChangeState(int key);

	 States currstate;

private:
	static StateManager* statemanager;

	StateManager();
};

#endif // !STATEMANAGER_H
