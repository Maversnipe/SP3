#ifndef DRILLPROJ_H
#define DRILLPROJ_H

#include "GameObject.h"
#include "Blocks.h"

class DrillProj : public GameObject
{
public:
	DrillProj();
	~DrillProj();

	void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt);
	void checkCollision(std::vector<GameObject *> &Objs, std::vector<Block *> &Blks);
};

#endif // !DrillProj_H
