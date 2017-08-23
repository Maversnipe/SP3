#include "Explosive.h"

Explosive::Explosive()
	: GameObject(GameObject::GO_EXPLOSION),
	m_fexplosiontime(1.f),
	m_felapsedtime(0.f),
	exploding(false)
{
}

Explosive::~Explosive()
{
}

void Explosive::Setexplosiontime(double dt)
{
	if (this->m_fexplosiontime == 0.f || !exploding)
	{
		this->m_felapsedtime = 0.f;
		this->m_fexplosiontime = dt + 3.f;
		exploding = true;
	}
}

void Explosive::Update(std::vector<GameObject*> objs, std::vector<Block*> blks, double dt)
{
	this->m_felapsedtime += dt;

	this->scale.x += 10.f * dt;
	this->scale.y += 10.f * dt;

	this->aabb.SetAABB(this->pos, this->scale);

	if (checkCollision(objs, blks))
		Response();

	if (this->m_felapsedtime > this->m_fexplosiontime)
	{

		this->active = false;
		this->m_fexplosiontime = 0;
		exploding = false;
	}
}

bool Explosive::checkCollision(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks)
{
	bool check = false;

	for (auto &i : Objs)
	{
		if (i == this)
			continue;

		check = CollisionManager::getCManager()->CheckCollisionC(this, i);

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
		check = CollisionManager::getCManager()->CheckCollisionC(this, i);

		if (check)
		{
			affected = i;
			break;
		}
	}

	return check;
}

void Explosive::Response()
{
	CollisionManager::getCManager()->CollisionResponseC(this, affected);
}