#include "WoodBlock.h"
#include "GameObject.h"

Woodblock::Woodblock(Grid* grid) : Block(grid)
{
	Btype = GameObject::BLOCK_TYPE::GO_WOOD;
}

Woodblock::~Woodblock()
{
}

void Woodblock::Update(double dt)
{
	this->torque.SetZero();

	// Check block's mass
	if (this->mass == 0)
		this->invmass = 0;
	else
		this->invmass = 1 / this->mass;

	// Check if block is on air to apply gravity
	if (isonAir && !onGround)
		this->vel.y += -9.8 * dt;

	// Check if block has health
	//if (this->getHealth() <= 0)
	//	this->active = false;

	// Apply gravity
	this->pos += this->vel * dt * 10;
	
	// Check and set if block is on air
	if (this->vel.y != 0)
		this->isonAir = true;
	else
	{
		this->isonAir = false;
		this->vel.y = 0;
		this->angularVelocity = 0;
	}

	// Set block's AABB
	this->aabb.SetAABB(this->pos, this->scale);

	// Block's collision response
	if (m_grid->CheckCollision(this, &affected))
	{
		Response();

		if (this->isonAir)
		{
			Vector3 pos(Math::Clamp((affected->pos - this->pos).x, -this->scale.x / 2, this->scale.x / 2), Math::Clamp((affected->pos - this->pos).y, -this->scale.x / 2, this->scale.y / 2), 0);
			Vector3 pos2(Math::Clamp((this->pos - affected->pos).x, -affected->scale.x / 2, affected->scale.x / 2), Math::Clamp((this->pos - affected->pos).y, -affected->scale.y / 2, affected->scale.y / 2), 0);

			if (abs(pos.y) > abs(pos.x))// && abs(this->pos.x - affected->pos.x) < this->scale.x)
			{
				pos += this->pos;

				if ((pos - affected->pos).Length() <= pos2.Length())
				{
					this->isonAir = false;
					this->vel.y = 0;

					//CollisionManager::getCManager()->PositionalCorrection(this, affected);
				}
			}

			//else if (abs(pos.y) < abs(pos.x))
			//{
			//	if (pos.x < 0)
			//	{
			//		Vector3 rightvector(-1, 0, 0);
			//		this->vel = this->vel - 2 * this->vel.Dot(rightvector) * rightvector;
			//	}
			//	else if (pos.x > 0)
			//	{
			//		Vector3 rightvector(1, 0, 0);
			//		this->vel = this->vel - 2 * this->vel.Dot(rightvector) * rightvector;
			//	}
			//}
			
		}
		else if (!isonAir)
		{
			this->pos += this->vel * dt * 10;
		}
	}
	else
	{
		this->isonAir = true;
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

	if (this->onGround)
		this->vel.y = 0;

	// Burning
	if (m_bBurning)
		BurnUpdate();
}

void Woodblock::BurnUpdate()
{

}
