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
	if ((int)mousepos.x % 8 >= 4)
	{
		gridx = (int)mousepos.x + (8 - (int)mousepos.x % 8);
	}
	else
	{
		gridx = (int)mousepos.x - ((int)mousepos.x % 8);
	}
	if ((int)mousepos.y % 8 >= 4)
	{
		gridy = (int)mousepos.y + (8 - (int)mousepos.y % 8);
	}
	else
	{
		gridy = (int)mousepos.y - ((int)mousepos.y % 8);
	}

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
			std::cout << blockList[i]->getHealth() << std::endl;
		}
	}
	return true;
}
