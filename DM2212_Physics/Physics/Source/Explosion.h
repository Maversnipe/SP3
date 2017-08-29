#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "Blocks.h"

class Explosion : public GameObject
{
public:
	Explosion();
	~Explosion();

	void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt);
	void checkCollision(std::vector<GameObject *> &Objs, std::vector<Block *> &Blks);
};

#endif