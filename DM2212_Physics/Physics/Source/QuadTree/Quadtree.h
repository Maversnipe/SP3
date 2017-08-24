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
	bool CollisionCheck(GameObject* GO);

private:
	static const int MAX_OBJECTS = 4; // Max objects ineach Quadtree
	int currNumOfObjects; // Current number of objects in Quadtree
	AABB m_boundary; // Bounds off quadtree

	Quadtree* NW; // Top left
	Quadtree* NE; // Top right
	Quadtree* SW; // Bottom left
	Quadtree* SE; // Bottom right

	std::vector<GameObject*> m_objects;
	Vector3 pos;
	float halfDist; // Half length of Quadtree
};
#endif