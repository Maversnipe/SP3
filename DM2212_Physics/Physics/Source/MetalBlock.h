#ifndef METALBLOCK_H
#define METALBLOCK_H

#include "Blocks.h"
class Metalblock : public Block
{
public:
	Metalblock();
	~Metalblock();

	void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt);
};

#endif //!METALBLOCK