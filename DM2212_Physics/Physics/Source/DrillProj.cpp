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
		this->pos += this->vel * static_cast<float>(dt);
	}
	this->aabb.SetAABB(this->pos, this->scale);
}