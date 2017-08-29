#ifndef CANNONBALL_H
#define CANNONBALL_H
#include "Blocks.h"

class Cannonball : public GameObject
{
public:
	Cannonball();
	~Cannonball();

	void Init();

	void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt);
	void checkCollision(std::vector<GameObject *> &Objs, std::vector<Block *> &Blks);
};

#endif // !CANNONBALL_H
