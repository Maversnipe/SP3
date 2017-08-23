#ifndef GRASSBLOCK_H
#define GRASSBLOCK_H

#include "Blocks.h"

class Grid;
class Grassblock : public Block
{
public:
	Grassblock(Quadtree* qtree, Grid* grid);
	~Grassblock();

	void Update(double dt);
	
};

#endif //!GRASSBLOCK