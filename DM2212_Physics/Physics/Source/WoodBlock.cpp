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

	//<Gravity Code>
	//if(isonAir)
	//	this->vel.y += -9.8f * dt;
	//</Gravity Code>

	// Check if block has health
	//if (this->getHealth() <= 0)
	//	this->active = false;

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
		this->pos += (this->vel + Vector3(0, -this->mass, 0)) * static_cast<float>(dt);
	}
	else
		this->pos += this->vel* static_cast<float>(dt);

	// Check and set if block is on air
	if (this->vel.y != 0 && this->vel.x != 0)
		this->isonAir = true;
	else
	{
		this->isonAir = false;
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
			Vector3 N(0, 1, 0);
			Vector3 pos(Math::Clamp((affected->pos - this->pos).x, -this->scale.x / 2, this->scale.x / 2), Math::Clamp((affected->pos - this->pos).y, -this->scale.x / 2, this->scale.y / 2), 0);
			Vector3 pos2(Math::Clamp((this->pos - affected->pos).x, -affected->scale.x / 2, affected->scale.x / 2), Math::Clamp((this->pos - affected->pos).y, -affected->scale.y / 2, affected->scale.y / 2), 0);
			if (pos.y < 0)
				N = -N;
			pos += this->pos;

			if ((pos - affected->pos).Length() < pos2.Length())
			{
				Vector3 right = N.Cross(Vector3(0, 0, 1));
				if (abs(pos.x) > abs(pos.y))
				{
					if (pos.Dot(right) > 0)
					{
						N = N.Cross(Vector3(0, 0, 1));
						N = -N;
					}
					if (pos.Dot(right) < 0)
					{
						N = N.Cross(Vector3(0, 0, 1));
					}
				}

				this->vel = this->vel - (2.f * this->vel.Dot(N)) * N;
				this->vel *= 0.65;
				this->isonAir = false;
			}
		}
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
