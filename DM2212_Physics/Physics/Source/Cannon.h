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

	void Update(double dt, Vector3 mousepos);

protected:
	float timer;
};

#endif // !CANNON_H
