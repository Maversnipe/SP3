#include "CannonTool.h"
#include "Blocks.h"
#include <iostream>
using std::cout;
using std::endl;

CannonTool::CannonTool()
{
	i_Price = 10;
	pos.Set(0, 0, 0);
	isSet = false;
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
	}

}

bool CannonTool::UseTool(vector<Block*> blockList, vector<GameObject*>& goList)
{
	if (!isSet)
	{
		isSet = true;
		cout << "Cannon Set at: " << pos << endl;
		return false;
	}
	else
	{
		GameObject *go = FetchGO(goList);

		go->type = GameObject::GO_BALL;
		go->tooltype = TOOL_TYPE::CANNONBALL;
		go->pos = pos;
		go->vel = dir * 50;
		go->scale.Set(2, 2, 2);
		go->aabb.SetAABB(go->pos, go->scale);

		isSet = false;
		return true;
	}
}
