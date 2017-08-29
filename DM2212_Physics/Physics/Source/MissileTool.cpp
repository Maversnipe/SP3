#include "MissileTool.h"
#include "Blocks.h"
#include <iostream>
#include "missile.h"
using std::cout;
using std::endl;

MissileTool::MissileTool() : ToolsInfo()
{
	i_Price = 50;
	pos.Set(0, 0, 0);
	toolproj = TOOL_PROJ::ROCKET;
	tooltype = GameObject::TOOL_TYPE::MISSILE;
}

MissileTool::~MissileTool()
{
}

void MissileTool::Init()
{
	i_Price = 50;
	pos.Set(0, 0, 0);
}

void MissileTool::Update(double dt, Vector3 mousepos)
{
	int gridx, gridy;
	if ((int)mousepos.x % 4 >= 2)
	{
		gridx = (int)mousepos.x + (4 - (int)mousepos.x % 4);
	}
	else
	{
		gridx = (int)mousepos.x - ((int)mousepos.x % 4);
	}
	if ((int)mousepos.y % 4 >= 2)
	{
		gridy = (int)mousepos.y + (4 - (int)mousepos.y % 4);
	}
	else
	{
		gridy = (int)mousepos.y - ((int)mousepos.y % 4);
	}

	pos = Vector3(gridx, gridy, 0);//update to mouse pos
}

bool MissileTool::UseTool(vector<Block*> blockList, vector<GameObject*> &goList)
{
	if (!isSet)
	{
		isSet = true;
		cout << "Missile Set at: " << pos << endl;
		return false;
	}
	else
	{
		GameObject *go = FetchGO(goList);
		missile* Missile = static_cast<missile*>(go);
		Missile->type = GameObject::GO_BALL;		
		Missile->toolproj = TOOL_PROJ::ROCKET;
		Missile->vel = dir * 50;
		Missile->scale.Set(2, 2, 2);
		Missile->pos.y = pos.y;
		Missile->pos.x = -Missile->scale.x;
		Missile->aabb.SetAABB(Missile->pos, Missile->scale);

		Missile->Init();
		isSet = false;
		return true;
	}
}
