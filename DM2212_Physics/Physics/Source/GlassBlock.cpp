#include "GlassBlock.h"
#include "GameObject.h"

Glassblock::Glassblock()
	: Block()
{
	Btype = GameObject::BLOCK_TYPE::GO_GLASS;

}

Glassblock::~Glassblock()
{
}

void Glassblock::Update(std::vector<GameObject*> objs, std::vector<Block*> blks, double dt)
{
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
	checkCollision(objs, blks);

	if (m_iHealth <= 1)
		this->block_status = GameObject::BLOCK_STATUS::DAMAGED;
}