#include "Blocks.h"

Block::Block()
	: GameObject(GameObject::GO_BLOCK),
	m_iHealth(0),
	m_bIsdestroyed(false),
	m_bDestructable(false),
	m_bIsdamaged(false)
{

}

Block::Block(int Type, int health, bool Destructable)
{
	m_iType = Type;
	m_iHealth = health;
	m_bDestructable = Destructable;
}

Block::~Block()
{
}

void Block::getDamaged(int damage)
{
	 m_iHealth -= damage;
	 std::cout << getHealth() << std::endl;
	 if (Isdestroyed())
	 {
		 active = false;
	 }
}
