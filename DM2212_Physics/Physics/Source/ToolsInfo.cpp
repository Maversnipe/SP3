#include "ToolsInfo.h"

ToolsInfo::ToolsInfo()
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

void ToolsInfo::UseTool(vector<GameObject*> goList)
{
}
