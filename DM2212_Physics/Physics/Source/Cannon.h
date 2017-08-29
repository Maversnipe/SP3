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

	bool GetFiring();
	void SetFiring(bool state);

private:
	int m_ibullet;		//Bullets
	bool m_bfiring;
};

#endif // !CANNON_H
