#include "PickaxeTool.h"
#include <iostream>
using std::cout;
using std::endl;

PickaxeTool::PickaxeTool()
{
	i_Price = 0;
	pos.Set(0, 0, 0);
}

PickaxeTool::~PickaxeTool()
{
}

void PickaxeTool::Init()
{
	i_Price = 0;
	pos.Set(0, 0, 0);
}

void PickaxeTool::Update(double dt)
{
	float m_worldHeight = 100.f;
	float m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float posX = static_cast<float>(x) / w * m_worldWidth;
	float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
	pos.Set(posX, posY, 0);//update to mouse pos
}

void PickaxeTool::UseTool(vector<GameObject*> goList)
{
	cout << "Paxe used at: "<< pos << endl;
	for (unsigned i = 0; i < goList.size(); ++i)
	{
		if ((goList[i]->pos - pos).LengthSquared() <= goList[i]->scale.x * goList[i]->scale.x && goList[i]->active && goList[i]->type == GameObject::GO_BLOCK)
		{
			goList[i]->active = false;//if pickaxe hits a grass, i cant access the brick to minus health
		}
	}
}
