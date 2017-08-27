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
	this->vel.SetZero();

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

	/*if (m_grid->CheckCollision(this, &affected))
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
	}*/

	/*if (m_grid->CheckCollision(this, &affected))
	{
		Response();
	}*/
}

void Cannonball::Update(std::vector<GameObject*> objs, std::vector<Block*> blks, double dt)
{
	//Check mass
	if (this->mass == 0)
		this->invmass = 0;
	else
		this->invmass = 1 / this->mass;

	if (this->active)
	{
		this->vel.y += -mass * dt;
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

	if (check)
		return check;

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