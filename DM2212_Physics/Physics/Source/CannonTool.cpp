#include "CannonTool.h"
#include "Blocks.h"
#include "SceneBase.h"
#include "PlayerInfo.h"
#include <iostream>
#include <sstream>

CannonTool::CannonTool() : ToolsInfo()
{
	i_Price = 10;
	pos.Set(0, 0, 0);
	m_bisSet = false;
	tooltype = TOOL_TYPE::CANNON;
	cannon = nullptr;
	cannonball = nullptr;
	m_breset = false;
}

CannonTool::~CannonTool()
{
}

void CannonTool::Init()
{
	i_Price = 10;
	pos.Set(0, 0, 0);
	m_bisSet = false;
	cannon = nullptr;
	cannonball = nullptr;
}

void CannonTool::Update(double dt, Vector3 mousepos)
{
	m_fcooldown -= dt;

	if (!m_bisSet)
	{
		i_Price = 10;
		pos = mousepos;//update to mouse pos
	}
	else
	{
		i_Price = 0;

		if (mousepos != pos)
		{
			this->dir.Set(mousepos.x - this->pos.x, mousepos.y - this->pos.y, 0);
			if (this->dir != Vector3(0, 0, 0))
				this->dir.Normalize();
			this->dir.y = Math::Clamp(std::abs(this->dir.y), sin(-270.f), sin(90.f));
		}

		if (m_breset == true)
		{
			cannon->Init();
			m_breset = false;
		}

		cannon->Update(dt, mousepos);

		std::ostringstream ss;
		ss.str(std::string());
		ss.precision(5);
		ss << "Bullets: " << cannon->Getbullet();
		std::string s = ss.str();
		PlayerInfo::GetInstance()->SetString(s);

		if (cannon->Getbullet() <= 0)
		{
			cannon->active = false;
			Init();
		}
	}
}

bool CannonTool::UseTool(vector<Block*> blockList, vector<GameObject*>& goList)
{
	if (!m_bisSet)
	{
		if (CheckPlacement(blockList))
		{
			std::cout << "Cannon Set at: " << pos << std::endl;

			//Spawn Cannon
			cannon = static_cast<Cannon*>(FetchGO(goList));
			cannon->type = GameObject::GO_CANNONT;
			cannon->pos = pos;
			cannon->pos.y += 1.f;
			cannon->vel.SetZero();
			cannon->scale.Set(10, 5, 1);
			cannon->aabb.SetAABB(cannon->pos, cannon->scale);
			cannon->Init();
			m_breset = true;
			m_bisSet = true;

			return true;
		}
	}
	else
	{
		//Time between shots
		if (m_fcooldown <= 0)
		{
			//Spawn Cannonball
			GameObject *go = FetchGO(goList);
			go->type = GameObject::GO_BALL;
			go->toolproj = TOOL_PROJ::CANNONBALL;
			go->pos = cannon->pos;
			go->vel = cannon->dir * 50;
			go->scale.Set(2, 2, 2);
			go->aabb.SetAABB(go->pos, go->scale);
			cannonball = static_cast<Cannonball*>(go);
			cannon->Decrease(1);
			m_fcooldown = 2.f;
		}
	}
	return false;
}

bool CannonTool::CheckPlacement(vector<Block*> blockList)
{
	bool check = false;         //Check if inside other blocks
	bool canplace = true;       //Check for suitable for placement

	for (auto &i : blockList)
	{
		if (!i->active)
			continue;

		check = CollisionManager::getCManager()->PtVsAABB(pos, i);

		//Collision check, prevent spawn on blocks
		if (check)
		{
			canplace = false;
			break;
		}

		//Check if its on Grassblock(has to be on grassblock)
		if (i->Btype == GameObject::GO_GRASS)
		{
			if (pos.y > i->aabb.GetMaxAABB().y + 1.f && pos.y <  i->aabb.GetMaxAABB().y + 3.f)
			{
				canplace = true;
			}
			else
			{
				canplace = false;
			}
		}
	}

	return canplace;
}

int CannonTool::GetAmmo()
{
	if (cannon != nullptr)
		return cannon->Getbullet();
	else
		return 0;
}