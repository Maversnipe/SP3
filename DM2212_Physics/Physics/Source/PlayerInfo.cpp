#include "PlayerInfo.h"
#include "PickaxeTool.h"
#include "CannonTool.h"
#include "DrillTool.h"
#include "ThumperTool.h"
#include "MissileTool.h"
#include "TestWeapon.h"
#include "DynamiteTool.h"

PlayerInfo *PlayerInfo::instance = 0;

PlayerInfo::PlayerInfo()
	:i_ActiveTool(0)
	, i_Money(100)
	, i_Score(0)
{
}

PlayerInfo::~PlayerInfo()
{
	if (ToolManager)
	{
		for (int i = 0; i < i_NumTools; i++)
		{
			delete ToolManager[i];
			ToolManager[i] = NULL;
		}
		delete[] ToolManager;
		ToolManager = NULL;
	}
}

void PlayerInfo::Init()
{
	i_NumTools = 6;
	ToolManager = new ToolsInfo*[i_NumTools];
	ToolManager[0] = new PickaxeTool();
	ToolManager[1] = new CannonTool();
	ToolManager[2] = new ThumperTool();
	ToolManager[3] = new DrillTool();
	ToolManager[4] = new MissileTool();
	ToolManager[5] = new DynamiteTool();
}

void PlayerInfo::Init(bool pickaxe, bool cannon, bool thumper, bool drill, bool dynamite, bool missile)
{
	i_NumTools = 0;
	ToolManager = new ToolsInfo*[6];
	if (pickaxe)
	{
		i_NumTools++;
		ToolManager[i_NumTools-1] = new PickaxeTool();
	}
	if (cannon)
	{
		i_NumTools++;
		ToolManager[i_NumTools-1] = new CannonTool();
	}
	if (thumper)
	{
		i_NumTools++;
		ToolManager[i_NumTools-1] = new ThumperTool();
	}
	if (drill)
	{
		i_NumTools++;
		ToolManager[i_NumTools-1] = new DrillTool();
	}
	if (missile)
	{
		i_NumTools++;
		ToolManager[i_NumTools-1] = new MissileTool();
	}
	if (dynamite)
	{
		i_NumTools++;
		ToolManager[i_NumTools-1] = new DynamiteTool();
	}
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
	ToolManager[i_ActiveTool]->scale.Set(8, 8, 1);
}

void PlayerInfo::UseCurrentTool(vector<Block*> blockList, vector<GameObject*> &goList)
{
	if (ToolManager[i_ActiveTool]->GetPrice() <= i_Money)
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
	if (i_ActiveTool > i_NumTools - 1)
		i_ActiveTool = 0;
	if (i_ActiveTool < 0)
		i_ActiveTool = i_NumTools - 1;
	std::cout << i_ActiveTool << std::endl;
}

void PlayerInfo::SetGold(int gold)
{
	i_Money = gold;
}

void PlayerInfo::AddGold(int amount_)
{
	i_Money += amount_;
}

ToolsInfo* PlayerInfo::GetActiveTool()
{
	return ToolManager[i_ActiveTool];
}

void PlayerInfo::SetString(std::string input)
{
	s_str = input;
}

std::string PlayerInfo::GetString()
{
	return s_str;
}
