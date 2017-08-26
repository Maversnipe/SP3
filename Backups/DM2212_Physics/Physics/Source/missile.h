#pragma once

#include "GameObject.h"
#include "Blocks.h"

class missile : public GameObject
{
public:
	missile(Grid* grid);
	~missile();

	void Init();
	void Update(Vector3 mousepos, double dt);
	void Response();

	bool touchCursor;
protected:
	GameObject* affected;
};

