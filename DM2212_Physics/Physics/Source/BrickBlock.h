#ifndef BRICK_BLOCK
#define BRICK_BLOCK

#include "Blocks.h"
class Grid;
class Brickblock : public Block
{
public:
	Brickblock(Grid* grid);
	~Brickblock();

	void Update(double dt);
};

#endif // !BRICK_BLOCK
