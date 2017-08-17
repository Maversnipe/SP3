#ifndef COLLISION_H
#define COLLISION_H

#include "Vector3.h"
#include <vector>

class AABB
{
public:
	bool AABBvsAABB(AABB other);

	//Circle
	bool CirclevsCircle(AABB other);

	//Getters
	Vector3 GetMaxAABB() { return max; };
	Vector3 GetMinAABB() { return min; };

protected:
	Vector3 min;
	Vector3 max;

	//Circle
	float radius;
	Vector3 position;
};

#endif // !COLLISION_H
