#pragma once
#include "ToolsInfo.h"
#include "Application.h"
#include "Cannon.h"

class CannonTool :public ToolsInfo
{
public:
	CannonTool(Grid * grid);
	~CannonTool();

	void Init();//set default data
	void Update(double dt, Vector3 mousepos);//upadate data
	bool UseTool(vector<Block*> blockList, vector<GameObject*> &goList);
private:
	bool isSet;
	Cannon* cannon;
};