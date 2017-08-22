#include "DrillProj.h"

DrillProj::DrillProj()
	: GameObject(GameObject::GO_BALL)
{
}

DrillProj::~DrillProj()
{
}

void DrillProj::Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt)
{
	if (this->active)
		this->pos += this->vel * static_cast<float>(dt);

	this->aabb.SetAABB(this->pos, this->scale);

	if (checkCollision(objs, blks))
	{
		Response();
	}
}

bool DrillProj::checkCollision(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks)
{
	bool check = false;

	//for (auto &i : Objs)
	//{
	//	if (i == this || !i->active)
	//		continue;

	//	check = CollisionManager::getCManager()->CheckCollisionC(this, i);

	//	if (check)
	//	{
	//		affected = i;
	//		break;
	//	}
	//}

	//if (check)
	//	return check;

	for (auto &i : Blks)
	{
		check = CollisionManager::getCManager()->CheckCollisionB(this, i);

		if (check)
		{
			affected = i;
			break;
		}
	}

	return check;
}

void DrillProj::Response()
{
	affected->getDamaged(5);
	CollisionManager::getCManager()->CollisionResponseB(this, affected);
}