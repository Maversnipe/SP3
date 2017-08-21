#include "WoodBlock.h"

Woodblock::Woodblock() : Block(2, 3, true)
{
}

Woodblock::~Woodblock()
{
}

void Woodblock::Update(std::vector<GameObject*> objs, std::vector<Block*> blks, double dt)
{
	// Check if block has health
	if (this->getHealth() <= 0)
		this->active = false;

	this->torque.SetZero();

	// Check block's mass
	if (this->mass == 0)
		this->invmass = 0;
	else
		this->invmass = 1 / this->mass;

	// Check if block is on air to apply gravity
	if (this->isonAir)
	{
		// Apply gravity
		this->pos += (this->vel + Vector3(0, -5, 0)) * static_cast<float>(dt);

	}
	else
		this->pos += this->vel* static_cast<float>(dt);

	// Check and set if block is on air
	if (this->vel.y != 0 || this->vel.x != 0)
		this->isonAir = true;
	else
		this->isonAir = false;

	// Set block's AABB
	this->aabb.SetAABB(this->pos, this->scale);

	// Block's collision response
	if (checkCollision(objs, blks))
	{
		Response();
		//this->getDamaged(1);
	}

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

	// Burning
	if (m_bBurning)
		BurnUpdate();
}

void Woodblock::BurnUpdate()
{

}
