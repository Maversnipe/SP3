#pragma once
#include "ToolsInfo.h"
#include "Application.h"
#include "Cannon.h"
#include "Cannonball.h"

class CannonTool :public ToolsInfo
{
public:
	CannonTool(Grid * grid);
	~CannonTool();

	void Init();//set default data
	void Update(double dt, Vector3 mousepos);							  //Update data(Cannon, icon pos)
	bool UseTool(vector<Block*> blockList, vector<GameObject*> &goList); //Shoot/Set Cannon
	bool CheckPlacement(vector<Block*> blockList);						//Check cannon can be placed
	int GetAmmo();

private:
	float m_fcooldown;		//Time between shots
	bool m_bisSet;			//Cannon Place check
	Cannon* cannon;			//Points to Cannon
	Cannonball* cannonball; //Points to Cannonball
	bool m_breset;			//Check for reposition
};