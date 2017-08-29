#pragma once

#include "GameObject.h"
#include "Blocks.h"

class missile : public GameObject
{
public:
	missile();
	~missile();

	void Init();
	void Update(std::vector<GameObject*>& Objs, std::vector<Block*>& Blks, Vector3 mousepos, double dt);
	bool checkCollision(std::vector<GameObject *> &Objs, std::vector<Block *> &Blks);

	bool touchCursor;
};

