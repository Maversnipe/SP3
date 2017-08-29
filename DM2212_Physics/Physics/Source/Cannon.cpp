#include "Cannon.h"

Cannon::Cannon() :
GameObject(GameObject::GO_CANNONT)
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
	//Invisible cannon w no collision
	//Projectile launches from this cannon to prevent from getting stuck
	//(collision is set on cannon icon placed)
	
	//Rotate to mousepos
	this->dir.Set(mousepos.x - this->pos.x,mousepos.y - this->pos.y, 0);
	if(this->dir != Vector3(0, 0, 0))
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