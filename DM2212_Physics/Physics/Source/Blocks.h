#ifndef BLOCKS_H
#define BLOCKS_H

#include "CollisionManager.h"
#include "GameObject.h"

class Block : public GameObject
{
public:
	Block();
	virtual ~Block();

	void Init();
	void checkCollision(std::vector<GameObject *> &Objs, std::vector<Block *> &Blks);

	int getHealth() { return m_iHealth; };										//Get health
	int getType() { return m_iType; };											//Get Type of block
	bool Isdestroyed() { if (m_iHealth <= 0) return true; else return false; };	//Get active or not
	bool Isdestructable() { return m_bDestructable; };							//Get if block can/cannot be destroyed
	void getDamaged(int damage);//damage block

	GameObject* FetchGO(std::vector<GameObject*>& goList);

protected:
	int m_iType;			//Type of blocks(0. Grass, 1. Glass, 2. Wood, 3. Metal)
	int m_iHealth;			//Hp
	bool m_bDestructable;	//can/cannot be destroyed
	bool m_bIsdestroyed;	//Is destroyed or not
	bool m_bIsdamaged;		//Is damaged or not
};

#endif // !BLOCKS_H