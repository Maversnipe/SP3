#ifndef WOODBLOCK_H
#define WOODBLOCK_H

#include "Blocks.h"

class Woodblock : public Block
{
public:
	Woodblock();
	~Woodblock();

protected:
	bool m_bBurning;
};

#endif //!WOODBLOCK