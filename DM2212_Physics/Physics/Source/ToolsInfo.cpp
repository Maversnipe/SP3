#include "ToolsInfo.h"

ToolsInfo::ToolsInfo(Quadtree* qtree, Grid* grid) : GameObject(qtree, grid, GameObject::GO_TOOLS)
{
}

ToolsInfo::~ToolsInfo()
{
}

void ToolsInfo::Init()
{
}

void ToolsInfo::Update(double dt, Vector3 mousepos)
{
}

void ToolsInfo::SetPrice(const int in_price)
{
	i_Price = in_price;
}

int ToolsInfo::GetPrice() const
{
	return i_Price;
}

void ToolsInfo::SetPos(const Vector3 in_pos)
{
	pos = in_pos;
}

Vector3 ToolsInfo::GetPos() const
{
	return pos;
}

bool ToolsInfo::UseTool(vector<Block*> blockList, vector<GameObject*> &goList)
{
	return false;
}

GameObject* ToolsInfo::FetchGO(vector<GameObject*>& goList)
{
	for (std::vector<GameObject *>::iterator it = goList.begin(); it != goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(m_qtree, m_grid, GameObject::GO_BALL);
		goList.push_back(go);
	}
	GameObject *go = goList.back();
	go->active = true;
	return go;
}