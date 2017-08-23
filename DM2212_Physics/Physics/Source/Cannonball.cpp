#include "Cannonball.h"
#include "GameObject.h"

Cannonball::Cannonball(Grid* grid)
	: GameObject(grid, GameObject::GO_BALL)
{
	Cannonball::pos = GameObject::pos;
}

Cannonball::~Cannonball()
{
}

void Cannonball::Update(double dt)
{
	if(this->active)
		this->pos += this->vel * static_cast<float>(dt);

	this->aabb.SetAABB(this->pos, this->scale);

	if (m_grid->CheckCollision(this, &affected))
	{
		Response();
	}
}

void Cannonball::Response()
{
	CollisionManager::getCManager()->CollisionResponseC(this, affected);
}