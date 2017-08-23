#ifndef GLASSBLOCK_H
#define GLASSBLOCK_H

#include "Blocks.h"

class Grid;
class Glassblock : public Block
{
public:
	Glassblock(Grid* grid);
	~Glassblock();

	void Update(double dt);
};

#endif //!GLASSBLOCK