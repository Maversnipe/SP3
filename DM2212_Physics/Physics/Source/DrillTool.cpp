#include "DrillTool.h"
#include "Blocks.h"
#include <iostream>
using std::cout;
using std::endl;

DrillTool::DrillTool()
{
	i_Price = 10;
	pos.Set(0, 0, 0);
	isSet = false;
	tooltype = TOOL_TYPE::DRILL;

}

DrillTool::~DrillTool()
{
}

void DrillTool::Init()
{
	i_Price = 10;//set price
	pos.Set(0, 0, 0);//default pos
	isSet = false;
}

void DrillTool::Update(double dt, Vector3 mousepos)
{
	if (!isSet)
	{
		pos = mousepos;//update to mouse pos
	}
	else
	{
		if (mousepos != pos)//change dir with mouse
		{
			dir = (mousepos - pos).Normalized();
			int i_dir = Math::RadianToDegree(atan2(dir.x, dir.y));
			if (i_dir > -90 - 45 && i_dir <= -90 + 45)
			{
				dir.Set(-1, 0, 0);
			}
			else if (i_dir > -45 && i_dir <= +45)
			{
				dir.Set(0, 1, 0);
			}
			else if (i_dir > 90 - 45 && i_dir <= 90 + 45)
			{
				dir.Set(1, 0, 0);
			}
			else
			{
				dir.Set(0, -1, 0);
			}

		}
	}

}

bool DrillTool::UseTool(vector<Block*> blockList, vector<GameObject*> &goList)
{
	if (!isSet)//set drill launcher
	{
		isSet = true;
		cout << "DrillTool Set at: " << pos << endl;
		return false;
	}
	else//Fire drill
	{
		GameObject *go = FetchGO(goList);
		go->type = GameObject::GO_BALL;
		go->toolproj = TOOL_PROJ::DRILLPROJ;
		go->pos = pos;
		go->vel = dir * 50;
		go->scale.Set(2, 2, 2);
		go->aabb.SetAABB(go->pos, go->scale);

		isSet = false;
		return true;
	}
}
