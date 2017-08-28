#ifndef CANNONBALL_H
#define CANNONBALL_H
#include "Blocks.h"

class Cannonball : public GameObject
{
public:
	Cannonball(Grid* grid);
	~Cannonball();

	void Init();

	void Update(double dt);
	void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt);
	bool checkCollision(std::vector<GameObject *> &Objs, std::vector<Block *> &Blks);
	void Response();

protected:
	GameObject* affected;
};

#endif // !CANNONBALL_H
