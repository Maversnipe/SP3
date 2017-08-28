#include "PickaxeTool.h"
#include "Blocks.h"
#include <iostream>
using std::cout;
using std::endl;

PickaxeTool::PickaxeTool(Grid* grid) : ToolsInfo(grid)
{
	i_Price = 0;
	pos.Set(0, 0, 0);
	tooltype = TOOL_TYPE::PICKAXE;
}

PickaxeTool::~PickaxeTool()
{
}

void PickaxeTool::Init()
{
	i_Price = 0;
	pos.Set(0, 0, 0);
}

void PickaxeTool::Update(double dt, Vector3 mousepos)
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
		int mouseposx, mouseposy;
		mouseposx = mousepos.x + 2;
		mouseposy = mousepos.y - 2;
	}
	gridx -= 2.f;
	gridy += 2.f;
	pos = Vector3(gridx, gridy, 0);//update to mouse pos
}

bool PickaxeTool::UseTool(vector<Block*> blockList, vector<GameObject*> &goList)
{
	cout << "Paxe used at: " << pos << endl;
	for (unsigned i = 0; i < blockList.size(); ++i)
	{
		if ((blockList[i]->pos - pos).LengthSquared() < blockList[i]->scale.x * blockList[i]->scale.x && blockList[i]->active)
		{
			blockList[i]->getDamaged(1);
		}
	}
	return true;
}
