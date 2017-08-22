#include "Blocks.h"

Block::Block()
	: GameObject(GameObject::GO_BLOCK),
	m_iHealth(0),
	m_iType((int)GameObject::Btype),
	m_bIsdestroyed(false),
	m_bDestructable(false),
	m_bIsdamaged(false),
	m_vec3dir2(Vector3(0, 1, 0))
{

}

Block::Block(int Type, int health, bool Destructable)
{
	m_iType = Type;
	m_iHealth = health;
	m_bDestructable = Destructable;
}

Block::~Block()
{
}

bool Block::checkCollision(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks)
{
	bool check = false;

	for (auto &i : Objs)
	{
		check = CollisionManager::getCManager()->CheckCollisionB(this, i);

		if (check)
		{
			affected = i;
			break;
		}
	}

	if (check)
		return check;

	for (auto &i : Blks)
	{
		if (i == this)
			continue;
		
		if (this->Btype == GameObject::BLOCK_TYPE::GO_GRASS && i->Btype == GameObject::BLOCK_TYPE::GO_GRASS && !this->isonAir)
			continue;

		check = CollisionManager::getCManager()->CheckCollisionB(this, i);

		if (check)
		{
			affected = i;
			break;
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
	 if (this->Isdestroyed())
	 {
		 active = false;
	 }
}
