#include "DrillProj.h"

DrillProj::DrillProj()
	: GameObject(GameObject::GO_BALL)
{
}

DrillProj::~DrillProj()
{
}

void DrillProj::Update(std::vector<GameObject*> objs, std::vector<Block*> blks, double dt)
{
	if (this->vel.x > 50)
	{
		this->vel.x = 50;
	}
	else if (this->vel.x < -50)
	{
		this->vel.x = -50;
	}
	if (this->vel.y > 50)
	{
		this->vel.y = 50;
	}
	else if (this->vel.y < -50)
	{
		this->vel.y = -50;
	}

	//Check mass
	if (this->mass == 0)
		this->invmass = 0;
	else
		this->invmass = 1 / this->mass;

	if (this->active)
	{
		this->vel.y += -9.8 * dt;
		this->pos += this->vel * static_cast<float>(dt);
	}
	this->aabb.SetAABB(this->pos, this->scale);

	checkCollision(objs, blks);
}

void DrillProj::checkCollision(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks)
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