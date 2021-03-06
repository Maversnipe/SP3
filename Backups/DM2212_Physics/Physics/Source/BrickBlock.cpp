#include "BrickBlock.h"
#include "GameObject.h"

Brickblock::Brickblock(Grid* grid) : Block(grid)
{
	Btype = GameObject::BLOCK_TYPE::GO_BRICK;
}

Brickblock::~Brickblock()
{
}

/*bool Block::checkCollision(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks)
{
	bool check = false;

	for (auto &i : Objs)
	{
		check = CollisionManager::getCManager()->CheckCollisionB(this, i);

		if (check)
		{
			CollisionManager::getCManager()->CollisionResponseB(this, i);
			//affected = i;
			//break;
		}
	}

	for (auto &i : Blks)
	{
		if (i == this)
			continue;
		
		if (this->Btype == GameObject::BLOCK_TYPE::GO_GRASS && i->Btype == GameObject::BLOCK_TYPE::GO_GRASS)
			continue;

		check = CollisionManager::getCManager()->CheckCollisionB(this, i);

		if (check)
		{
			CollisionManager::getCManager()->CollisionResponseB(this, i);
			//affected = i;
			//break;
		}
	}

	return check;
}*/

void Brickblock::Update(double dt)
{
	//if (this->getHealth() <= 0)
	//	this->active = false;

	this->torque.SetZero();

	// Check block's mass
	if (this->mass == 0)
		this->invmass = 0;
	else
		this->invmass = 1 / this->mass;

	// Apply gravity
	this->vel.y += -mass * dt;
	this->pos += this->vel * dt * 5;

	// Set block's AABB
	this->aabb.SetAABB(this->pos, this->scale);

	// Block's collision response
	//if (checkCollision(objs, blks))
	//{
	//	//Response();
	//	//this->getDamaged(1);
	//}

	//if (m_grid->CheckCollision(this, &affected))
	//{
	//	Response();
	//	//this->getDamaged(1);
	//}

	//Rotation
	if (!this->torque.IsZero())
	{
		this->momentOfInertia = this->mass * 1 * 1;
		float aa = this->torque.z * (1 / this->momentOfInertia);
		this->angularVelocity += aa * static_cast<float>(dt);
		if (this->angularVelocity > 10)
			this->angularVelocity = 10;
	}

	else
	{
		if (this->angularVelocity > 0)
		{
			this->angularVelocity += -0.1 * dt;

			if (this->angularVelocity <= 0)
				this->angularVelocity = 0;
		}
		else if (this->angularVelocity < 0)
		{
			this->angularVelocity += 0.1 * dt;

			if (this->angularVelocity >= 0)
				this->angularVelocity = 0;
		}
	}

	float theta = atan2(this->dir.y, this->dir.x);
	theta += this->angularVelocity * static_cast<float>(dt);
	this->dir.Set(cos(theta), sin(theta), 0);

	if (!this->dir.IsZero())
		this->dir.Normalize();
}
