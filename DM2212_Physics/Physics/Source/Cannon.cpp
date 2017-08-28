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
	this->dir.Set(mousepos.x - this->pos.x,mousepos.y - this->pos.y, 0);
	this->dir.Normalize();
	this->dir.y = Math::Clamp(std::abs(this->dir.y), sin(-270.f), sin(90.f));
}

int Cannon::Getbullet()
{
	return m_ibullet;
}

void Cannon::Decrease(int amount)
{
	m_ibullet -= amount;
}
