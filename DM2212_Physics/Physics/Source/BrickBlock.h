#ifndef BRICK_BLOCK
#define BRICK_BLOCK

#include "Blocks.h"
class Brickblock : public Block
{
public:
	Brickblock(Grid* grid);
	~Brickblock();

	void Update(double dt);
	void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt);
};

#endif // !BRICK_BLOCK
