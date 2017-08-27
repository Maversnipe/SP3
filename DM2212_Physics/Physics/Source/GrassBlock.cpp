#include "GrassBlock.h"
#include "GameObject.h"

Grassblock::Grassblock(Grid* grid) : Block(grid)
{
	Btype = GameObject::BLOCK_TYPE::GO_GRASS;
	this->mass = 0;
}

Grassblock::~Grassblock()
{
}

void Grassblock::Update(double dt)
{
	mass = 0;

	this->torque.SetZero();

	if (this->mass == 0)
		this->invmass = 0;
	else
		this->invmass = 1 / this->mass;

	this->aabb.SetAABB(this->pos, this->scale);

	if (m_grid->CheckCollision(this, &affected))
	{
		//Response();
		//this->getDamaged(1);
	}

	//if (checkCollision(objs, blks))
	//{
	//	//Response();
	//	//this->getDamaged(1);
	//}
}
