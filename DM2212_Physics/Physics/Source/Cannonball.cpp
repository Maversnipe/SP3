#include "Cannonball.h"

Cannonball::Cannonball()
	: GameObject(GameObject::GO_BALL)
{
	Cannonball::pos = GameObject::pos;
}

Cannonball::~Cannonball()
{
}

void Cannonball::Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt)
{
	if(this->active)
		this->pos += this->vel * static_cast<float>(dt);

	this->aabb.SetAABB(this->pos, this->scale);

	if (checkCollision(objs, blks))
	{
		Response();
	}
}

bool Cannonball::checkCollision(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks)
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

void Cannonball::Response()
{
	CollisionManager::getCManager()->CollisionResponseC(this, affected);
}