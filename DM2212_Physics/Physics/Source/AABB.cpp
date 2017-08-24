#include "AABB.h"

void AABB::SetAABB(Vector3 pos, Vector3 size)
{
	position = pos;
	max = pos + size * (0.5, 0.5, 0.5);
	min = pos - size * (0.5, 0.5, 0.5);
	SetRadius(size.x);
}

bool AABB::ContainsPoint(Vector3 point)
{
	if (point.x < GetMaxAABB().x && point.x > GetMinAABB().x)
	{ // Checks if X axis of point is within AABB
		if (point.y < GetMaxAABB().y && point.y > GetMinAABB().y)
		{ // Checks if Y axis of point is within AABB
			return true;
		}
	}
	return false;
}

bool AABB::IntersectsAABB(AABB other)
{
	if (GetMaxAABB().x > other.GetMinAABB().x && GetMinAABB().x < other.GetMaxAABB().x)
	{ // Checks if either this.Max.x > other.Min.x,
		// or if this.Min.x < other.Max.x
		if (GetMaxAABB().y > other.GetMinAABB().y && GetMinAABB().y < other.GetMaxAABB().y)
		{
			return true;
		}
	}
	return false;
}
