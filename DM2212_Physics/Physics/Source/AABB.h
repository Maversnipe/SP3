#ifndef AABB_H
#define AABB_H

#include "Vector3.h"
#include <vector>

class AABB
{
public:
	//Set
	void SetRadius(float r) { radius = r; };
	void SetAABB(Vector3 pos, Vector3 size);

	//Getters
	Vector3 GetMaxAABB() { return max; };
	Vector3 GetMinAABB() { return min; };
	Vector3 GetPos() { return position;};
	float GetRadius() { return radius; };
	
	// For Quadtree
	bool ContainsPoint(Vector3 point);
	bool IntersectsAABB(AABB other);

protected:
	Vector3 min;
	Vector3 max;

	//Circle
	float radius;
	Vector3 position;
};

#endif // !AABB_H
