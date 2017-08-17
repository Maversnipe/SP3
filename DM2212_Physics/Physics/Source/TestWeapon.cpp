#include "TestWeapon.h"
#include "Blocks.h"
#include <iostream>
using std::cout;
using std::endl;

TestWeapon::TestWeapon()
{
	i_Price = 10;
	pos.Set(0, 0, 0);
}

TestWeapon::~TestWeapon()
{
}

void TestWeapon::Init()
{
	i_Price = 10;
	pos.Set(0, 0, 0);
}

void TestWeapon::Update(double dt, Vector3 mousepos)
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

void TestWeapon::UseTool(vector<GameObject*> goList)
{
	cout << "TestTool used at: " << pos << endl;
	for (unsigned i = 0; i < goList.size(); ++i)
	{
		Block* b = static_cast<Block*>(goList[i]);

		if ((goList[i]->pos - pos).LengthSquared() < goList[i]->scale.x * goList[i]->scale.y && goList[i]->active && goList[i]->type == GameObject::GO_BLOCK)
		{
			b->getDamaged(1);//if pickaxe hits a grass, i cant access the brick to minus health
		}
	}

}
