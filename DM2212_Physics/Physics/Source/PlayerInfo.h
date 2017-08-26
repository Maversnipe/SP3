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

	void Init(Grid* grid);

	void Update(double dt, Vector3 mousepos);//use weapons in here
	void UseCurrentTool(vector<Block*> blockList, vector<GameObject*> &goList);

	int GetActiveToolIndex()const;
	void SetActiveToolIndex(int ToolIndex);

	void SetGold(int gold);
	ToolsInfo* GetActiveTool();

private:
	int i_Money;
	int i_Score;

	//tool stuff
	const int i_NumTools = 6;
	int i_ActiveTool;//index of the active tool e.g pickaxe index = 0;
	ToolsInfo** ToolManager;

};