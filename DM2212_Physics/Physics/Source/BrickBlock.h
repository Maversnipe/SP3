#ifndef BRICK_BLOCK
#define BRICK_BLOCK

#include "Blocks.h"
class Brickblock : public Block
{
public:
	Brickblock();
	~Brickblock();

	void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt);
};

#endif // !BRICK_BLOCK
