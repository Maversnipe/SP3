#pragma once
#include "ToolsInfo.h"
#include "Application.h"
#include "Blocks.h"

class PickaxeTool :public ToolsInfo
{
public:
	PickaxeTool();
	~PickaxeTool();

	void Init();//set default data
	void Update(double dt, Vector3 mousepos);//upadate data
	bool UseTool(vector<Block*> blockList, vector<GameObject*>& goList);
private:

};