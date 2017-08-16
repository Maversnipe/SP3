#pragma once
#include <iostream>
#include "ToolsInfo.h"
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

	void Update();//use weapons in here

private:
	int i_Money;
	int i_Score;

	//tool stuff
	const int i_NumTools = 2;
	int i_ActiveTool;//index of the active tool e.g pickaxe index = 0;
	ToolsInfo** ToolManager;
	
};