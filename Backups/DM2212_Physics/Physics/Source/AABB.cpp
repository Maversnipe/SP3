#include "AABB.h"

void AABB::SetAABB(Vector3 pos, Vector3 size)
{
	position = pos;
	max = pos + size * (0.5, 0.5, 0.5);
	min = pos - size * (0.5, 0.5, 0.5);
	SetRadius(size.x);
}