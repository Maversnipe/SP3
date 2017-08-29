#include "GrassBlock.h"
#include "GameObject.h"

Grassblock::Grassblock() : Block()
{
	Btype = GameObject::BLOCK_TYPE::GO_GRASS;
	this->mass = 0;
}

Grassblock::~Grassblock()
{
}

void Grassblock::Update(std::vector<GameObject*> objs, std::vector<Block*> blks, double dt)
{
	mass = 0;
	vel.SetZero();

	if (this->mass == 0)
		this->invmass = 0;
	else
		this->invmass = 1 / this->mass;

	this->aabb.SetAABB(this->pos, this->scale);

	checkCollision(objs, blks);
}
