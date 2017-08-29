#include "TestWeapon.h"
#include "Blocks.h"
#include <iostream>
using std::cout;
using std::endl;

TestWeapon::TestWeapon() : ToolsInfo()
{
	i_Price = 10;
	pos.Set(0, 0, 0);
	isSet = false;
	tooltype = TOOL_TYPE::DRILL;

}

TestWeapon::~TestWeapon()
{
}

void TestWeapon::Init()
{
	i_Price = 10;
	pos.Set(0, 0, 0);
	isSet = false;
}

void TestWeapon::Update(double dt, Vector3 mousepos)
{
	if (!isSet)
	{
		pos = mousepos;//update to mouse pos
	}
	else
	{
		if (mousepos != pos)
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

bool TestWeapon::UseTool(vector<Block*> blockList, vector<GameObject*> &goList)
{
	if (!isSet)
	{
		isSet = true;
		cout << "TestTool Set at: " << pos << endl;
		return false;
	}
	else
	{
		GameObject *go = FetchGO(goList);
		go->type = GameObject::GO_BALL;//to be changed
		go->toolproj = TOOL_PROJ::DRILLPROJ;
		go->pos = pos;
		go->vel = dir * 50;
		go->scale.Set(2, 2, 2);
		go->aabb.SetAABB(go->pos, go->scale);
		isSet = false;
		return true;
	}
}
