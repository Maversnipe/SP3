#ifndef DYNAMITE_TOOL_H
#define DYNAMITE_TOOL_H
#include "ToolsInfo.h"
#include "Application.h"

class DynamiteTool : public ToolsInfo
{
public:
	DynamiteTool(Grid* grid);
	~DynamiteTool();

	void Init();
	void Update(double dt, Vector3 mousepos);
	bool UseTool(vector<Block*> blockList, vector<GameObject*> &goList);
private:
	bool isSet;
};


#endif