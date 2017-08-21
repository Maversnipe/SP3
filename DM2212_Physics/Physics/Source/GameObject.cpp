#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue, BLOCK_TYPE Btyp)
	: type(typeValue),
	scale(1, 1, 1),
	restitution(1),
	dir(1, 0, 0),
	active(false),
	mass(1.f),
	momentOfInertia(mass * (scale.x * 0.5) * (scale.x * 0.5)),
	angularVelocity(0.0f),
	isonAir(false),
	iscolliding(false),
	rotation(0.0f),
	torque(0, 0, 0),
	tooltype(TOOL_TYPE::NONE)
{
	if (typeValue == GO_BLOCK)
		Btype = Btyp;
}

GameObject::~GameObject()
{
}