#ifndef CANNON_H
#define CANNON_H

#include "GameObject.h"
#include "Vector3.h"
#include "SpatialPartitioning\Grid.h"

class Cannon : public GameObject
{
public:
	Cannon(Grid* grid);
	~Cannon();

	void Init();
	void Update(double dt, Vector3 mousepos);

	int Getbullet();
	void Decrease(int amount);

protected:
	float m_ftimer;		//Despawn time
	int m_ibullet;		//Bullets
};

#endif // !CANNON_H
