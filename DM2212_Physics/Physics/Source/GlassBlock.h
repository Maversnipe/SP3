#ifndef GLASSBLOCK_H
#define GLASSBLOCK_H

#include "Blocks.h"

class Glassblock : public Block
{
public:
	Glassblock(Grid* grid);
	~Glassblock();

	void Update(double dt);
	void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt);
};

#endif //!GLASSBLOCK