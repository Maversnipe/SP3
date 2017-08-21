#ifndef GLASSBLOCK_H
#define GLASSBLOCK_H

#include "Blocks.h"
#include "GameObject.h"

class Glassblock : public Block
{
public:
	Glassblock();
	~Glassblock();

	void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt);
};

#endif //!GLASSBLOCK