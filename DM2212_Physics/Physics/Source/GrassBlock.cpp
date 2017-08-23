#include "GrassBlock.h"
#include "GameObject.h"

Grassblock::Grassblock(Grid* grid) : Block(grid)
{
}

Grassblock::~Grassblock()
{
}

void Grassblock::Update(double dt)
{	
	this->torque.SetZero();

	if (this->mass == 0)
		this->invmass = 0;
	else
		this->invmass = 1 / this->mass;

	/*if (this->isonAir)
	{
		this->pos += (this->vel + Vector3(0, -5, 0)) * static_cast<float>(dt);

	}
	else
		this->pos += this->vel* static_cast<float>(dt);*/

	if (this->vel.y != 0 || this->vel.x != 0)
		this->isonAir = true;
	else
		this->isonAir = false;

	this->aabb.SetAABB(this->pos, this->scale);

	if (m_grid->CheckCollision(this, &affected))
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

	/*if (go2->getHealth() <= 2)
		go->Btype = GameObject::BLOCK_TYPE::GO_WOODDAM;*/
}
