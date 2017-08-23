#ifndef QUADTREE_H
#define QUADTREE_H
#include <vector>
#include "Vector3.h"
#include "../AABB.h"

class GameObject;
class Quadtree
{
public:
	Quadtree();
	Quadtree(AABB boundary);
	~Quadtree();

	bool Insert(GameObject* to_insert);
	void Subdivide();
	std::vector<GameObject*> QueryRange(AABB range);
private:
	static const int MAX_OBJECTS = 4;
	int currNumOfObjects;
	AABB m_boundary;

	Quadtree* NW;
	Quadtree* NE;
	Quadtree* SW;
	Quadtree* SE;

	std::vector<GameObject*> m_objects;
	Vector3 pos;
};
#endif