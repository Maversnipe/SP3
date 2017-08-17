#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "GameObject.h"
#include "Collision.h"
#include "Vector3.h"
#include <vector>
#include <iostream>
#include <algorithm>

struct Manifold
{
	GameObject* A;
	GameObject* B;
	float penetration;
	Vector3 normal;
	Vector3 outvel;
};

class CollisionManager
{
public:
	void SetWorldSize(int height, int width);

	void Check(std::vector<GameObject *> &Objs, double dt);
	bool CheckCollisionC(GameObject *go1, GameObject *go2, float dt);
	bool CheckCollisionB(GameObject *go1, GameObject *go2, float dt);
	void CollisionResponseC(GameObject *go, GameObject *go2, float dt);
	void CollisionResponseB(GameObject *go, GameObject *go2);

	void PositionalCorrection(GameObject *go, GameObject *go2);
private:
	//Collision checks Methods
	bool AABBvsAABB(Manifold* m);
	bool SweepAABBs(Manifold* m);

	bool CirclevsCircle(Manifold* m);

	bool AABBvsCircle(Manifold* m);

protected:
	int m_iworld_height;
	int m_iworld_width;
	Manifold *m = new Manifold;
	float colltimecheck=0;

	//"Gravity"
	Vector3 m_vec3Gravity = Vector3(0, -9.8, 0);
};

#endif // !COLLISION_MANAGER_H
