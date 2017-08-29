#pragma once
#include <iostream>
#include "ToolsInfo.h"
#include "GameObject.h"
#include <vector>
#include <string>

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
	void Init(bool pickaxe,bool cannon, bool thumper, bool drill, bool dynamite, bool missile);

	void Update(double dt, Vector3 mousepos);//use weapons in here
	void UseCurrentTool(vector<Block*> blockList, vector<GameObject*> &goList);

	int GetActiveToolIndex()const;
	void SetActiveToolIndex(int ToolIndex);

	int GetGold() { return i_Money; }
	void AddGold(int amount);

	void SetGold(int gold);
	ToolsInfo* GetActiveTool();

	void SetString(std::string input);
	std::string GetString();

private:
	int i_Money;		//Money
	int i_Score;		//For timer
	std::string s_str;	//RenderTextonScreen(bullets,etc)

	//tool stuff
	int i_NumTools;
	int i_ActiveTool;//index of the active tool e.g pickaxe index = 0;
	ToolsInfo** ToolManager;

};