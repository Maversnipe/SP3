#include "PlayerInfo.h"
#include "PickaxeTool.h"

PlayerInfo *PlayerInfo::instance = 0;

PlayerInfo::PlayerInfo()
	:i_ActiveTool(0)
	, i_Money(0)
	, i_Score(0)
{
}

PlayerInfo::~PlayerInfo()
{
}

void PlayerInfo::Init()
{
	ToolManager = new ToolsInfo*[i_NumTools];
	ToolManager[0] = new PickaxeTool();


}

void PlayerInfo::Update(double dt)
{
	ToolManager[i_ActiveTool]->Update(dt);
}

void PlayerInfo::UseCurrentTool(vector<GameObject*> goList)
{
	ToolManager[i_ActiveTool]->UseTool(goList);
}
