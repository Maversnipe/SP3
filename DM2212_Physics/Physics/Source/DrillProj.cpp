#include "DrillProj.h"

DrillProj::DrillProj(Quadtree* qtree, Grid* grid)
	: GameObject(qtree, grid, GameObject::GO_BALL)
{
}

DrillProj::~DrillProj()
{
}

void DrillProj::Update(double dt)
{
	if (this->active)
		this->pos += this->vel * static_cast<float>(dt);

	this->aabb.SetAABB(this->pos, this->scale);

	//if (m_grid->CheckCollision(this, &affected))
	//{
	//	Response();
	//}
}

void DrillProj::Response()
{/*
	affected->getDamaged(5);*/
	CollisionManager::getCManager()->CollisionResponseB(this, affected);
}