#include "PickaxeTool.h"
#include <iostream>
using std::cout;
using std::endl;

PickaxeTool::PickaxeTool()
{
	i_Price = 0;
	v3_Position.Set(0, 0, 0);
}

PickaxeTool::~PickaxeTool()
{
}

void PickaxeTool::Init()
{
	i_Price = 0;
	v3_Position.Set(0, 0, 0);
}

void PickaxeTool::Update(double dt)
{
	double xpos, ypos;
	Application::GetCursorPos(&xpos, &ypos);
	v3_Position.Set(xpos, ypos, 0);//update to mouse pos
}

void PickaxeTool::UseTool()
{
	cout << "Paxe used at: "<<v3_Position << endl;
}
