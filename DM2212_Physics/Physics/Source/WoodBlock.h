#ifndef WOODBLOCK_H
#define WOODBLOCK_H
#include "Blocks.h"

class Grid;
class Woodblock : public Block
{
public:
	Woodblock(Quadtree* qtree, Grid* grid);
	~Woodblock();

	void Update(double dt);
	void BurnUpdate();

protected:
	bool m_bBurning;
};

#endif //!WOODBLOCK