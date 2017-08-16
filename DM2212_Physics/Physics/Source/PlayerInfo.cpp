#include "PlayerInfo.h"
#include "PickaxeTool.h"

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

void PlayerInfo::Update()
{
}
