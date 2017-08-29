#ifndef WOODBLOCK_H
#define WOODBLOCK_H
#include "Blocks.h"

class Woodblock : public Block
{
public:
	Woodblock();
	~Woodblock();

	void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt);
};

#endif //!WOODBLOCK