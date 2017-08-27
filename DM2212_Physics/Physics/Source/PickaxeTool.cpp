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
	int mouseposx, mouseposy;
	mouseposx = mousepos.x + 2;
	mouseposy = mousepos.y - 2;
	if ((int)mouseposx % 8 >= 4)
	{
		gridx = (int)mouseposx + (8 - (int)mouseposx % 8);
	}
	else
	{
		gridx = (int)mouseposx - ((int)mouseposx % 8);
	}
	if ((int)mouseposy % 8 >= 4)
	{
		gridy = (int)mouseposy + (8 - (int)mouseposy % 8);
	}
	else
	{
		gridy = (int)mouseposy - ((int)mouseposy % 8);
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
			std::cout << blockList[i]->getHealth() << std::endl;
		}
	}
	return true;
}
