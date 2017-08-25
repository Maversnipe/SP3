#pragma once
#include "ToolsInfo.h"
#include "Application.h"
#include "Blocks.h"

class ThumperTool :public ToolsInfo
{
public:
	ThumperTool(Grid* grid);
	~ThumperTool();

	void Init();//set default data
	void Update(double dt, Vector3 mousepos);//upadate data
	bool UseTool(vector<Block*> blockList, vector<GameObject*>& goList);
private:

};