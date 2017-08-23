#ifndef CANNONBALL_H
#define CANNONBALL_H
#include "Blocks.h"

class Grid;
class Cannonball : public GameObject
{
public:
	Cannonball(Grid* grid);
	~Cannonball();

	void Update(double dt);
	void Response();

protected:
	GameObject* affected;
};

#endif // !CANNONBALL_H
