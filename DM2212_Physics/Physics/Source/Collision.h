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

	//Set
	void SetRadius(float r) { radius = r; };
	void SetAABB(Vector3 pos, Vector3 size);

	//Getters
	Vector3 GetMaxAABB() { return max; };
	Vector3 GetMinAABB() { return min; };
	float GetRadius() { return radius; };

protected:
	Vector3 min;
	Vector3 max;

	//Circle
	float radius;
	Vector3 position;
};

#endif // !COLLISION_H
