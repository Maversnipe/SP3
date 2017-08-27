#include "Blocks.h"
#include "PlayerInfo.h"

Block::Block(Grid* grid)
	: GameObject(grid, GameObject::GO_BLOCK),
	m_iHealth(0),
	m_iType((int)GameObject::Btype),
	m_bIsdestroyed(false),
	m_bDestructable(false),
	m_bIsdamaged(false)
{
}

Block::~Block()
{
}

void Block::Init()
{
	switch (Btype)
	{
	case GameObject::GO_GRASS:
		this->m_iHealth = 999;
		this->m_bDestructable = false;
		break;
	case GameObject::GO_GLASS:
		this->m_iHealth = 1;
		this->m_bDestructable = true;
		break;
	case GameObject::GO_WOOD:
		this->m_iHealth = 3;
		this->m_bDestructable = true;
		break;
	case GameObject::GO_METAL:
		this->m_iHealth = 999;
		this->m_bDestructable = false;
		break;
	case GameObject::GO_BRICK:
		this->m_iHealth = 5;
		this->m_bDestructable = true;
		break;
	}
}

bool Block::checkCollision(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks)
{
	bool check = false;

	for (auto &i : Objs)
	{
		if (!i->active)
			continue;

		check = CollisionManager::getCManager()->CheckCollisionB(this, i);

		if (check)
		{
			CollisionManager::getCManager()->CollisionResponseB(this, i);

			if (this->Btype != GameObject::GO_GRASS)
			{
				this->getDamaged(1);
			}

			//affected = i;
			//break;
		}
	}

	for (auto &i : Blks)
	{
		if (!i->active)
			continue;

		if (i == this)
			continue;

		if (this->Btype == GameObject::BLOCK_TYPE::GO_GRASS && i->Btype == GameObject::BLOCK_TYPE::GO_GRASS)
			continue;

		check = CollisionManager::getCManager()->CheckCollisionB(this, i);

		if (check)
		{
			CollisionManager::getCManager()->CollisionResponseB(this, i);
			//affected = i;
			//break;
		}
	}

	return check;
}

void Block::Response()
{
	if (affected->toolproj == GameObject::TOOL_PROJ::DRILLPROJ)
		return;

	CollisionManager::getCManager()->CollisionResponseB(this, affected);
}

void Block::getDamaged(int damage)
{
	this->m_iHealth -= damage;
	//std::cout << this->getHealth() << std::endl;
	if (this->Isdestroyed() && this->Isdestructable())
	{
		this->active = false;
		this->vel.SetZero();
		PlayerInfo::GetInstance()->AddGold(5);
	}
}
