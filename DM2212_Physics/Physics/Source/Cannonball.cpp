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
	Cannonball::vel.SetZero();
}

void Cannonball::Update(double dt)
{
	if (this->vel.x > 30)
	{
		this->vel.x = 30;
	}
	else if (this->vel.x < -30)
	{
		this->vel.x = -30;
	}
	if (this->vel.y > 30)
	{
		this->vel.y = 30;
	}
	else if (this->vel.y < -30)
	{
		this->vel.y = -30;
	}

	//Check mass
	if (this->mass == 0)
		this->invmass = 0;
	else
		this->invmass = 1 / this->mass;

	if (this->active)
	{
		this->vel.y += -9.8 * dt;
		this->pos += this->vel * static_cast<float>(dt);
	}
	this->aabb.SetAABB(this->pos, this->scale);

	if (m_grid->CheckCollision(this))
	{
		//Response();
	}
}

void Cannonball::Update(std::vector<GameObject*> objs, std::vector<Block*> blks, double dt)
{
	if (this->vel.x > 30)
	{
		this->vel.x = 30;
	}
	else if (this->vel.x < -30)
	{
		this->vel.x = -30;
	}
	if (this->vel.y > 30)
	{
		this->vel.y = 30;
	}
	else if (this->vel.y < -30)
	{
		this->vel.y = -30;
	}

	//Check mass
	if (this->mass == 0)
		this->invmass = 0;
	else
		this->invmass = 1 / this->mass;

	if (this->active)
	{
		this->vel.y += -9.8 * dt;
		this->pos += this->vel * static_cast<float>(dt);
	}
	this->aabb.SetAABB(this->pos, this->scale);

	if (checkCollision(objs, blks))
	{
		//Response();
	}
}

bool Cannonball::checkCollision(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks)
{
	bool check = false;

	for (auto &i : Objs)
	{
		if (i == this || !i->active)
			continue;

		check = CollisionManager::getCManager()->CheckCollisionC(this, i);

		if (check)
		{
			affected = i;
			Response();
			//break;
		}
	}

	for (auto &i : Blks)
	{
		if (!i->active)
			continue;

		check = CollisionManager::getCManager()->CheckCollisionC(this, i);

		if (check)
		{
			affected = i;
			Response();
			//break;
		}
	}

	return check;
}

void Cannonball::Response()
{
	CollisionManager::getCManager()->CollisionResponseC(this, affected);
}