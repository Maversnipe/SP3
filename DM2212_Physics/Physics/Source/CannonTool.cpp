#include "CannonTool.h"
#include "Blocks.h"
#include <iostream>
using std::cout;
using std::endl;

CannonTool::CannonTool(Grid* grid) : ToolsInfo(grid)
{
	i_Price = 10;
	pos.Set(0, 0, 0);
	isSet = false;
	tooltype = TOOL_TYPE::CANNON;

}

CannonTool::~CannonTool()
{
}

void CannonTool::Init()
{
	i_Price = 10;
	pos.Set(0, 0, 0);
	isSet = false;
}

void CannonTool::Update(double dt, Vector3 mousepos)
{
	if (!isSet)
	{
		pos = mousepos;//update to mouse pos
	}
	else
	{
		if (mousepos != pos)
			dir = (mousepos - pos).Normalized();

		cannon->Update(dt, mousepos);
	}
}

bool CannonTool::UseTool(vector<Block*> blockList, vector<GameObject*>& goList)
{
	if (!isSet)
	{
		isSet = true;
		cout << "Cannon Set at: " << pos << endl;
		
		//Spawn Cannon
		GameObject *go2 = FetchGO(goList);
		go2->type = GameObject::GO_CANNON;
		go2->pos = pos;
		go2->pos.y += 1.f;
		go2->vel.SetZero();
		go2->scale.Set(10, 5, 1);
		go2->aabb.SetAABB(go2->pos, go2->scale);
		m_grid->Add(go2);
		cannon = static_cast<Cannon*>(go2);
		cannon->Init();
		
		return true;
	}
	else
	{
		//Spawn Cannonball
		GameObject *go = FetchGO(goList);
		go->type = GameObject::GO_BALL;
		go->toolproj = TOOL_PROJ::CANNONBALL;
		go->pos = cannon->pos;
		go->vel = -cannon->dir * 50;
		go->scale.Set(2, 2, 2);
		go->aabb.SetAABB(go->pos, go->scale);
		cannon->Decrease(1);
		m_grid->Add(go);

		if (cannon->Getbullet() == 0)
		{
			cannon->active = false;
			isSet = false;
		}

		return false;
	}
}

bool CannonTool::CheckPlacement(vector<Block*> blockList)
{
	bool check = false;

	for (auto &i : blockList)
	{
		if (!i->active)
			continue;
	}
	return check;
}
