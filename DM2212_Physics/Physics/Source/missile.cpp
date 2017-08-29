#include "missile.h"

missile::missile() 
	: GameObject(GameObject::GO_BALL)
{
	this->aabb.SetAABB(this->pos, this->scale); //set collision area
}


missile::~missile()
{
}

void missile::Init()
{
	touchCursor = false;
}

void missile::Update(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks, Vector3 mousepos,double dt)
{
	if (!this->active)
		return;

	if (this->active) //update movement
	{
		if (this->mass == 0)
			this->invmass = 0;
		else
			this->invmass = 1 / this->mass;

		if ((this->pos - mousepos).Length() > this->scale.x && !this->touchCursor)
		{
			if ((this->pos - mousepos).Length() < 10)
			{
				this->touchCursor = true;
			}
			this->vel = (mousepos - this->pos).Normalized() * 50.f;
		}

		this->pos += this->vel * static_cast<float>(dt);

		this->aabb.SetAABB(this->pos, this->scale);
	}
}

bool missile::checkCollision(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks)
{
	bool check = false;

	for (auto &i : Objs)
	{
		if (i == this || !i->active)
			continue;

		check = CollisionManager::getCManager()->CheckCollisionC(this, i);
	}
	
	for (auto &i : Blks)
	{
		if (!i->active)
			continue;

		check = CollisionManager::getCManager()->CheckCollisionC(this, i);
	}
	return check;
}