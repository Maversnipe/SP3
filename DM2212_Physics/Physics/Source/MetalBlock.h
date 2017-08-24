#ifndef METALBLOCK_H
#define METALBLOCK_H

#include "Blocks.h"
class Grid;
class Metalblock : public Block
{
public:
	Metalblock(Quadtree* qtree, Grid* grid);
	~Metalblock();

	void Update(double dt);
};

#endif //!METALBLOCK