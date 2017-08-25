#include "missile.h"

missile::missile(Grid* grid) 
	: GameObject(grid, GameObject::GO_BALL)
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

void missile::Update(Vector3 mousepos,double dt)
{
	if (this->active) //update movement
	{
		if ((this->pos - mousepos).Length() > this->scale.x && !this->touchCursor)
		{
			if ((this->pos - mousepos).Length() < 10)
			{
				this->touchCursor = true;
			}
			this->vel = (mousepos - this->pos).Normalized() * 50;
		}

		this->pos += this->vel * static_cast<float>(dt);
	}
}

void missile::Response()
{
    //affected->getDamaged(5);
	CollisionManager::getCManager()->CollisionResponseB(this, affected);
}