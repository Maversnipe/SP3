#include "Collision.h"

bool AABB::AABBvsAABB(AABB other)
{
	if (max.x < other.min.x || min.x > other.max.x)
		return false;
	if (max.y < other.min.y || min.y > other.max.y)
		return false;

	return true;
}

bool AABB::CirclevsCircle(AABB other)
{
	float r = radius + other.radius;
	r *= r;
	return r < pow((position.x + other.position.x), 2) + pow((position.y + other.position.y), 2);
}

void AABB::SetAABB(Vector3 pos, Vector3 size)
{
	max = pos + size * (0.5, 0.5, 0.5);
	min = pos - size * (0.5, 0.5, 0.5);
}
