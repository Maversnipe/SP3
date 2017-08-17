#pragma once
#include "ToolsInfo.h"
#include "Application.h"

class TestWeapon :public ToolsInfo
{
public:
	TestWeapon();
	~TestWeapon();

	void Init();//set default data
	void Update(double dt, Vector3 mousepos);//upadate data
	void UseTool(vector<GameObject*> goList);
private:
	bool isSet;
};