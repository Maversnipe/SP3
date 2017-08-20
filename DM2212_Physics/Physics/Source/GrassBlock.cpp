#include "GrassBlock.h"

Grassblock::Grassblock() : Block(3, 1, true)
{
}

Grassblock::~Grassblock()
{
}

void Grassblock::Update(GameObject * go, double dt)
{
	Grassblock* go2 = static_cast<Grassblock*>(go);
	
	if (go2->getHealth() <= 0)
		go->active = false;

	if (go2->getHealth() <= 2)
		go->Btype = GameObject::BLOCK_TYPE::GO_WOODDAM;
}
