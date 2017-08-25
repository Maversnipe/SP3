#include "Cannonball.h"
#include "GameObject.h"

Cannonball::Cannonball(Grid* grid)
	: GameObject(grid, GameObject::GO_BALL)
{
}

Cannonball::~Cannonball()
{
}

void Cannonball::Init()
{
	Cannonball::pos = GameObject::pos;
	Cannonball::mass = 3.f;
	Cannonball::scale = Vector3(3.f, 3.f, 1.f);
}

void Cannonball::Update(double dt)
{
	if (this->active)
		this->pos += this->vel * static_cast<float>(dt);

	if (this->isonAir)
	{
		// Apply gravity
		this->pos += (this->vel + Vector3(0, -9.8, 0)) * static_cast<float>(dt);
	}
	else
		this->pos += this->vel* static_cast<float>(dt);

	if (this->vel.y != 0 && this->vel.x != 0)
		this->isonAir = true;
	else
	{
		this->isonAir = false;
	}

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