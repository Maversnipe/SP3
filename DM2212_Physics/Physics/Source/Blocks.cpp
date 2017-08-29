#include "Blocks.h"
#include "PlayerInfo.h"

Block::Block()
	: GameObject(GameObject::GO_BLOCK),
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
	this->block_status = GameObject::BLOCK_STATUS::FULL_HEALTH;
	switch (Btype)
	{
	case GameObject::GO_GRASS:
		this->m_iHealth = 999;
		this->m_bDestructable = false;
		break;
	case GameObject::GO_GLASS:
		this->m_iHealth = 2;
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

void Block::checkCollision(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks)
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

			if (i->toolproj == GameObject::TOOL_PROJ::ROCKET)
			{
				i->active = false;
				GameObject *go = FetchGO(Objs);
				go->type = GameObject::GO_EXPLOSION;
				go->toolproj = TOOL_PROJ::EXPLOSION;
				go->pos = this->pos;
				go->vel.SetZero();
				go->scale.Set(2, 2, 2);
				go->aabb.SetAABB(go->pos, go->scale);
			}

			if (this->Btype != GameObject::GO_GRASS)
			{
				this->getDamaged(1);
			}
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
		}
	}
}

void Block::getDamaged(int damage)
{
	this->m_iHealth -= damage;
	
	if (this->Isdestroyed() && this->Isdestructable())
	{
		this->active = false;
		this->vel.SetZero();
		PlayerInfo::GetInstance()->AddGold(5);
	}
}

GameObject* Block::FetchGO(std::vector<GameObject*>& goList)
{
	for (std::vector<GameObject *>::iterator it = goList.begin(); it != goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_BALL);
		goList.push_back(go);
	}
	GameObject *go = goList.back();
	go->active = true;
	return go;
}