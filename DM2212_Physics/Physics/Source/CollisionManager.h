#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "GameObject.h"
#include "Vector3.h"
#include <vector>
#include <iostream>
#include <algorithm>

//Organiser
struct Manifold
{
	GameObject* A;
	GameObject* B;
	float penetration;
	Vector3 normal;
};

class CollisionManager
{
public:
	static CollisionManager* getCManager();
	//Init
	void SetWorldSize(int height, int width);

	//"Update"
	//void Check(std::vector<GameObject *> &Objs, std::vector<Block *> &Blks, double dt);
	
	//Circular to other obj
	bool CheckCollisionC(GameObject *go1, GameObject *go2);
	//Box to other obj
	bool CheckCollisionB(GameObject *go1, GameObject *go2);
	//Circular to other obj
	void CollisionResponseC(GameObject *go, GameObject *go2);
	//Box to other obj
	void CollisionResponseB(GameObject *go, GameObject *go2);

	//Prevent Overlaps(objects)
	void PositionalCorrection(GameObject *go, GameObject *go2);


	~CollisionManager();

private:
	CollisionManager();
	static CollisionManager* cm;

	//Collision checks Methods
	bool AABBvsAABB(Manifold* m);

	bool CirclevsCircle(Manifold* m);

	bool AABBvsCircle(Manifold* m);

protected:
	//Playable Area
	int m_iworld_height;
	int m_iworld_width;

	Manifold *m = new Manifold;

	float colltimecheck = 0;

	//"Gravity"
	Vector3 m_vec3Gravity = Vector3(0, -9.8, 0);
};

#endif // !COLLISION_MANAGER_H
