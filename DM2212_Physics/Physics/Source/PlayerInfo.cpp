#include "PlayerInfo.h"
#include "PickaxeTool.h"
#include "CannonTool.h"
#include "DrillTool.h"
#include "ThumperTool.h"
#include "TestWeapon.h"

PlayerInfo *PlayerInfo::instance = 0;

PlayerInfo::PlayerInfo()
	:i_ActiveTool(0)
	, i_Money(100)
	, i_Score(0)
{
}

PlayerInfo::~PlayerInfo()
{
}

void PlayerInfo::Init(Quadtree* qtree, Grid* grid)
{
	ToolManager = new ToolsInfo*[i_NumTools];
	ToolManager[0] = new PickaxeTool(qtree, grid);
	ToolManager[1] = new CannonTool(qtree, grid);
	ToolManager[2] = new DrillTool(qtree, grid);
	ToolManager[3] = new ThumperTool(qtree, grid);
	ToolManager[4] = new TestWeapon(qtree, grid);

}

void PlayerInfo::Update(double dt, Vector3 mousepos)
{
	//switch tools
	{
		static bool isQ = false;
		if (Application::IsKeyPressed('Q') && !isQ)
			isQ = true;
		else if (!Application::IsKeyPressed('Q') && isQ)
		{
			SetActiveToolIndex(i_ActiveTool - 1);
			isQ = false;
		}
		static bool isE = false;
		if (Application::IsKeyPressed('E') && !isE)
			isE = true;
		else if (!Application::IsKeyPressed('E') && isE)
		{
			SetActiveToolIndex(i_ActiveTool + 1);
			isE = false;
		}
	}

	ToolManager[i_ActiveTool]->Update(dt, mousepos);
}

void PlayerInfo::UseCurrentTool(vector<Block*> blockList, vector<GameObject*> &goList)
{
	if (ToolManager[i_ActiveTool]->GetPrice() < i_Money)
	{
		if (ToolManager[i_ActiveTool]->UseTool(blockList, goList))
			i_Money -= ToolManager[i_ActiveTool]->GetPrice();

		std::cout << i_Money << std::endl;
	}
	else
	{
		std::cout << "NO MONEY" << std::endl;
	}
}

int PlayerInfo::GetActiveToolIndex()const
{
	return i_ActiveTool;
}

void PlayerInfo::SetActiveToolIndex(int ToolIndex)
{
	i_ActiveTool = ToolIndex;
	if (i_ActiveTool >= i_NumTools)
		i_ActiveTool = 0;
	if (i_ActiveTool == -1)
		i_ActiveTool = i_NumTools - 1;
	std::cout << i_ActiveTool << std::endl;
}

void PlayerInfo::SetGold(int gold)
{
	i_Money = gold;
}

ToolsInfo* PlayerInfo::GetActiveTool()
{
	return ToolManager[i_ActiveTool];
}
