#ifndef CANNONBALL_H
#define CANNONBALL_H
#include "Blocks.h"

class Grid;
class Cannonball : public GameObject
{
public:
	Cannonball(Quadtree* qtree, Grid* grid);
	~Cannonball();

	void Init();

	void Update(double dt);
	void Response();

protected:
	GameObject* affected;
};

#endif // !CANNONBALL_H
