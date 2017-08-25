#ifndef GRASSBLOCK_H
#define GRASSBLOCK_H

#include "Blocks.h"

class Grassblock : public Block
{
public:
	Grassblock(Grid* grid);
	~Grassblock();

	void Update(double dt);
	
};

#endif //!GRASSBLOCK