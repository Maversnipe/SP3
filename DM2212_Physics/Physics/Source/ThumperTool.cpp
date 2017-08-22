#include "ThumperTool.h"
#include "Blocks.h"
#include <iostream>
using std::cout;
using std::endl;

ThumperTool::ThumperTool()
{
	i_Price = 0;
	pos.Set(0, 0, 0);
	tooltype = TOOL_TYPE::THUMPER;
}

ThumperTool::~ThumperTool()
{
}

void ThumperTool::Init()
{
	i_Price = 0;
	pos.Set(0, 0, 0);
}

void ThumperTool::Update(double dt, Vector3 mousepos)
{
	pos = mousepos;//update to mouse pos
}

bool ThumperTool::UseTool(vector<Block*> blockList, vector<GameObject*> &goList)
{
	cout << "Thumper used at: " << pos << endl;
	//for (unsigned i = 0; i < blockList.size(); ++i)
	//{
	//	if ((blockList[i]->pos - pos).LengthSquared() < blockList[i]->scale.x * blockList[i]->scale.x && blockList[i]->active)
	//	{
	//		blockList[i]->getDamaged(1);
	//	}
	//}
	return true;
}
