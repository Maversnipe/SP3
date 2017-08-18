#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue, BLOCK_TYPE Btyp)
	: type(typeValue),
	scale(1, 1, 1),
	dir(0, 1, 0),
	restitution(1),
	active(false),
	mass(1.f),
	momentOfInertia(mass * (scale.x * 0.5) * (scale.x * 0.5)),
	angularVelocity(0.0f),
	isonAir(true),
	iscolliding(false),
	rotation(0.0f)
{
	if (typeValue == GO_BLOCK)
		Btype = Btyp;
}

GameObject::~GameObject()
{
}