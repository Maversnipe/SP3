#include "WoodBlock.h"

Woodblock::Woodblock() : Block(2, 3, true)
{
}

Woodblock::~Woodblock()
{
}

void Woodblock::Update(std::vector<GameObject*> objs, std::vector<Block*> blks, double dt)
{
	if (this->getHealth() <= 0)
		this->active = false;

	if (this->mass == 0)
		this->invmass = 0;
	else
		this->invmass = 1 / this->mass;

	if (this->isonAir)
	{
		this->pos += (this->vel + Vector3(0, -5, 0)) * static_cast<float>(dt);
	}
	else
		this->pos += this->vel* static_cast<float>(dt);

	if (this->vel.y != 0 || this->vel.x != 0)
		this->isonAir = true;
	else
		this->isonAir = false;

	this->aabb.SetAABB(this->pos, this->scale);

	if (checkCollision(objs, blks))
	{
		Response();
		//this->getDamaged(1);
	}
}
