#include "Cannon.h"

Cannon::Cannon(Grid* grid) :
GameObject(grid, GameObject::GO_CANNON)
{
}

Cannon::~Cannon()
{
}

void Cannon::Update(double dt, Vector3 mousepos)
{
	//Rotate to mousepos
	this->dir.Set(this->pos.x - mousepos.x, this->pos.y - mousepos.y, 0);
	this->dir.Normalize();
	this->dir.y = Math::Clamp(std::abs(this->dir.y), cos(Math::DegreeToRadian(-90.f)), cos(Math::DegreeToRadian(180.f)));
}
