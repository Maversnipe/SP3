#include "BrickBlock.h"
#include "GameObject.h"

Brickblock::Brickblock(Grid* grid) : Block(grid)
{
	Btype = GameObject::BLOCK_TYPE::GO_BRICK;
}

Brickblock::~Brickblock()
{
}

void Brickblock::Update(double dt)
{
	vel.SetZero();
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

	if (m_iHealth == 4)
		this->block_status = GameObject::BLOCK_STATUS::DAMAGED;
	else if (m_iHealth == 2)
		this->block_status = GameObject::BLOCK_STATUS::BROKEN;
}

void Brickblock::Update(std::vector<GameObject*> objs, std::vector<Block*> blks, double dt)
{
	if (this->getHealth() <= 0)
		this->active = false;

	if (this->vel.x > 5)
	{
		this->vel.x = 5;
	}
	else if (this->vel.x < -5)
	{
		this->vel.x = -5;
	}
	if (this->vel.y > 5)
	{
		this->vel.y = 5;
	}
	else if (this->vel.y < -5)
	{
		this->vel.y = -5;
	}

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
	if (checkCollision(objs, blks))
	{
		//Response();
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

	if (m_iHealth <= 4 && m_iHealth > 2)
		this->block_status = GameObject::BLOCK_STATUS::DAMAGED;
	else if (m_iHealth <= 2)
		this->block_status = GameObject::BLOCK_STATUS::BROKEN;
}
