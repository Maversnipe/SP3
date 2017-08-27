#include "GlassBlock.h"
#include "GameObject.h"

Glassblock::Glassblock(Grid* grid)
	: Block(grid)
{
	Btype = GameObject::BLOCK_TYPE::GO_GLASS;

}

Glassblock::~Glassblock()
{
}

void Glassblock::Update(double dt)
{
	//if (this->getHealth() <= 0)
	//	this->active = false;

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
	//normal
	//if (checkCollision(objs, blks))
	//{
	//	//Response();
	//	//this->getDamaged(1);
	//}
	//grid
	if (m_grid->CheckCollision(this, &affected))
	{
		//Response();
	}

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