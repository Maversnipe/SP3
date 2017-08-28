#include "Explosion.h"

Explosion::Explosion(Grid* grid)
	: GameObject(grid, GameObject::GO_BALL)
{
}

Explosion::~Explosion()
{
}

void Explosion::Update(double dt)
{
	if (!this->active)
		return;

	this->pos += this->vel * static_cast<float>(dt);

	this->scale;

	this->aabb.SetAABB(this->pos, this->scale);

	if (m_grid->CheckCollision(this, &affected))
	{

	}
}