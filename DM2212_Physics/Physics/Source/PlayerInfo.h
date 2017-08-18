#pragma once
#include <iostream>
#include "ToolsInfo.h"
#include "GameObject.h"
#include <vector>

using std::vector;
class PlayerInfo
{
protected:
	static PlayerInfo *instance;
	PlayerInfo();

public:
	//singleton sutff
	static PlayerInfo *GetInstance()
	{
		if (!instance)
			instance = new PlayerInfo;
		return instance;
	}
	static bool DropInstance()
	{
		if (instance)
		{
			delete instance;
			instance = NULL;
			return true;
		}
		return false;
	}

	~PlayerInfo();

	void Init();

	void Update(double dt,Vector3 mousepos);//use weapons in here
	void UseCurrentTool(vector<GameObject*> goList);

	int GetActiveToolIndex()const;
	void SetActiveToolIndex(int ToolIndex);

private:
	int i_Money;
	int i_Score;

	//tool stuff
	const int i_NumTools = 3;
	int i_ActiveTool;//index of the active tool e.g pickaxe index = 0;
	ToolsInfo** ToolManager;
	
};