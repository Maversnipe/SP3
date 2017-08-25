#pragma once

#include "GameObject.h"
#include "Blocks.h"

class Grid;
class missile : public GameObject
{
public:
	missile(Quadtree* qtree, Grid* grid);
	~missile();

	void Init();
	void Update(Vector3 mousepos, double dt);
	void Response();

	bool touchCursor;
protected:
	GameObject* affected;
};

