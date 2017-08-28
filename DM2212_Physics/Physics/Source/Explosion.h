#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "Blocks.h"

class Explosion : public GameObject
{
public:
	Explosion(Grid* grid);
	~Explosion();

	void Update(double dt);

protected:
	GameObject* affected;
};

#endif