#ifndef METALBLOCK_H
#define METALBLOCK_H

#include "Blocks.h"
class Grid;
class Metalblock : public Block
{
public:
	Metalblock(Grid* grid);
	~Metalblock();

	void Update(double dt);
};

#endif //!METALBLOCK