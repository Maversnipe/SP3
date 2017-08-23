#pragma once
#include "ToolsInfo.h"
#include "Application.h"

class Grid;
class Quadtree;
class DrillTool :public ToolsInfo
{
public:
	DrillTool(Quadtree* qtree, Grid* grid);
	~DrillTool();

	void Init();//set default data
	void Update(double dt, Vector3 mousepos);//upadate data
	bool UseTool(vector<Block*> blockList, vector<GameObject*> &goList);
private:
	bool isSet;
};
