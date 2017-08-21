#include "PickaxeTool.h"
#include "Blocks.h"
#include <iostream>
using std::cout;
using std::endl;

PickaxeTool::PickaxeTool()
{
	i_Price = 0;
	pos.Set(0, 0, 0);
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
	pos = mousepos;//update to mouse pos
}

bool PickaxeTool::UseTool(vector<Block*> blockList, vector<GameObject*> &goList)
{
	cout << "Paxe used at: "<< pos << endl;
	for (unsigned i = 0; i < blockList.size(); ++i)
	{
		if ((blockList[i]->pos - pos).LengthSquared() < blockList[i]->scale.x * blockList[i]->scale.x && blockList[i]->active)
		{
			blockList[i]->getDamaged(1);
		}
	}
	return true;
}
