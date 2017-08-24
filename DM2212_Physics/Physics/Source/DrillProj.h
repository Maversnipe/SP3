#ifndef DRILLPROJ_H
#define DRILLPROJ_H

#include "GameObject.h"
#include "Blocks.h"

class Grid;
class DrillProj : public GameObject
{
public:
	DrillProj(Quadtree* qtree, Grid* grid);
	~DrillProj();

	void Update(double dt);
	void Response();

protected:
	GameObject* affected;
};

#endif // !DrillProj_H
