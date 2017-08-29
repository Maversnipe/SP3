#pragma once
#include "ToolsInfo.h"
#include "Application.h"

class MissileTool :public ToolsInfo
{
public:
	MissileTool();
	~MissileTool();

	void Init();//set default data
	void Update(double dt, Vector3 mousepos);//upadate data
	bool UseTool(vector<Block*> blockList, vector<GameObject*>& goList);
private:
	bool isSet;
};