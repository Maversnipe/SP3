#include "Blocks.h"

Block::Block(Grid* grid)
	: GameObject(grid, GameObject::GO_BLOCK),
	m_iHealth(0),
	m_iType((int)GameObject::Btype),
	m_bIsdestroyed(false),
	m_bDestructable(false),
	m_bIsdamaged(false),
	m_vec3dir2(Vector3(0, 1, 0))
{
}

Block::~Block()
{
}

void Block::Response()
{
	if (affected->toolproj == GameObject::TOOL_PROJ::DRILLPROJ)
		return;

	CollisionManager::getCManager()->CollisionResponseB(this, affected);
}

void Block::getDamaged(int damage)
{
	 this->m_iHealth -= damage;
	 //std::cout << this->getHealth() << std::endl;
	 if (this->Isdestroyed())
	 {
		 active = false;
	 }
}
