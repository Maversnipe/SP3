#include "Cannon.h"

Cannon::Cannon(Grid* grid) :
GameObject(grid, GameObject::GO_CANNON)
{
}

Cannon::~Cannon()
{
}

void Cannon::Init()
{
	m_ibullet = 5;
}

void Cannon::Update(double dt, Vector3 mousepos)
{
	//Rotate to mousepos
	this->dir.Set(this->pos.x - mousepos.x, this->pos.y - mousepos.y, 0);
	this->dir.Normalize();
	this->dir.y = Math::Clamp(std::abs(this->dir.y), sin(-90.f), sin(180.f));
}

int Cannon::Getbullet()
{
	return m_ibullet;
}

void Cannon::Decrease(int amount)
{
	m_ibullet -= amount;
}
