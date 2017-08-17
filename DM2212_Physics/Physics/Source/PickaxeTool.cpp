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

void PickaxeTool::UseTool(vector<GameObject*> goList)
{
	cout << "Paxe used at: "<< pos << endl;
	for (unsigned i = 0; i < goList.size(); ++i)
	{
		Block* b = static_cast<Block*>(goList[i]);

		if ((goList[i]->pos - pos).LengthSquared() < goList[i]->scale.x * goList[i]->scale.y && goList[i]->active && goList[i]->type == GameObject::GO_BLOCK)
		{
			b->getDamaged(1);//if pickaxe hits a grass, i cant access the brick to minus health
		}
	}
}
