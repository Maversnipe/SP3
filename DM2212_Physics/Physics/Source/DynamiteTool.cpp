#include "DynamiteTool.h"
#include "Blocks.h"
#include <iostream>

DynamiteTool::DynamiteTool() : ToolsInfo()
{
	tooltype = TOOL_TYPE::DYNAMITE;
	isSet = false;

}

DynamiteTool::~DynamiteTool()
{
}

void DynamiteTool::Init()
{
	i_Price = 10;
	pos.Set(0.f, 0.f, 0.f);
}

void DynamiteTool::Update(double dt, Vector3 mousepos)
{
	if (!isSet)
	{
		pos = mousepos;
	}
}

bool DynamiteTool::UseTool(vector<Block*> blockList, vector<GameObject*>& goList)
{
	if (!isSet)
	{
		isSet = true;
		return false;
	}
	else
	{
		GameObject *go = FetchGO(goList);

		go->type = GameObject::GO_EXPLOSION;
		go->toolproj = TOOL_PROJ::EXPLOSION;
		go->pos = pos;
		go->vel.SetZero();
		go->scale.Set(2, 2, 2);
		go->aabb.SetAABB(go->pos, go->scale);
		isSet = false;

		return true;
	}
}