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
	void Update(double dt, Vector3 mousepos);//upadate data(Cannon, icon pos)
	bool UseTool(vector<Block*> blockList, vector<GameObject*> &goList); //Shoot/Set Cannon
	bool CheckPlacement(vector<Block*> blockList);


private:
	bool isSet;
	Cannon* cannon;
};