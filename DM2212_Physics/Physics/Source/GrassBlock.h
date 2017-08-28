#ifndef GRASSBLOCK_H
#define GRASSBLOCK_H

#include "Blocks.h"

class Grassblock : public Block
{
public:
	Grassblock(Grid* grid);
	~Grassblock();

	void Update(double dt);
	void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt);
};

#endif //!GRASSBLOCK