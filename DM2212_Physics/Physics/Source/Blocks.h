#ifndef BLOCKS_H
#define BLOCKS_H

#include "GameObject.h"
#include "CollisionManager.h"

class Block : public GameObject
{
public:
	Block();
	Block(int Type, int health, bool Destructable);								//Set Blocks
	virtual ~Block();

	//virtual void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt) {}

	bool checkCollision(std::vector<GameObject *> &Objs, std::vector<Block *> &Blks);
	void Response();

	int getHealth() { return m_iHealth; };										//Get health
	int getType() { return m_iType; };											//Get Type of block
	Vector3 getdir() { return m_vec3dir2; };
	void setdir(Vector3 vec) { m_vec3dir2 = vec; };
	bool Isdestroyed() { if (m_iHealth < 0) return true; else return false; };	//Get active or not
	void getDamaged(int damage);//damage block

protected:
	int m_iType;			//Type of blocks(0. Grass, 1. Glass, 2. Wood, 3. Metal)
	int m_iHealth;			//Hp
	bool m_bDestructable;	//can/cannot be destroyed
	bool m_bIsdestroyed;	//Is destroyed or not
	bool m_bIsdamaged;		//Is damaged or not
	Vector3 m_vec3dir2;

	GameObject* affected;
};

#endif // !BLOCKS_H
