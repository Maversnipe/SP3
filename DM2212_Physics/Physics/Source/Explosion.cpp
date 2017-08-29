#include "Explosion.h"

Explosion::Explosion()
	: GameObject(GameObject::GO_BALL)
{
}

Explosion::~Explosion()
{
}

void Explosion::Update(std::vector<GameObject*> objs, std::vector<Block*> blks, double dt)
{
	//Check mass
	if (this->mass == 0)
		this->invmass = 0;
	else
		this->invmass = 1 / this->mass;

	if (this->active)
	{

	}
	this->aabb.SetAABB(this->pos, this->scale);

	checkCollision(objs, blks);
}

void Explosion::checkCollision(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks)
{
	bool check = false;

	for (auto &i : Objs)
	{
		if (i == this || !i->active)
			continue;

		check = CollisionManager::getCManager()->CheckCollisionC(this, i);

		if (check)
		{
			CollisionManager::getCManager()->CollisionResponseC(this, i);
		}
	}

	for (auto &i : Blks)
	{
		if (!i->active)
			continue;

		check = CollisionManager::getCManager()->CheckCollisionC(this, i);

		if (check)
		{
			CollisionManager::getCManager()->CollisionResponseC(this, i);
		}
	}
}