#ifndef GRASSBLOCK_H
#define GRASSBLOCK_H

#include "Blocks.h"

class Grassblock : public Block
{
public:
	Grassblock();
	~Grassblock();

	void Update(GameObject* go, double dt);
	
};

#endif //!GRASSBLOCK