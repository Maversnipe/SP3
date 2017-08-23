#ifndef BRICK_BLOCK
#define BRICK_BLOCK

#include "Blocks.h"
class Grid;
class Brickblock : public Block
{
public:
	Brickblock(Quadtree* qtree, Grid* grid);
	~Brickblock();

	void Update(double dt);
};

#endif // !BRICK_BLOCK
