#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "AABB.h"
#include "SpatialPartitioning\Grid.h"

class GameObject
{
	friend Grid;
public:
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_SHIP,
		GO_ASTEROID,
		GO_BULLET,
		GO_ENEMY,
		GO_ENEMY_BULLET,
		GO_MISSILE,
		GO_POWERUP,
		GO_WALL,
		GO_PILLAR,

		//sp3 ^-^
		GO_TEST_ANIMATION,
		GO_BLOCK,
		GO_EXPLOSION,
		GO_TOOLS,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;

	enum BLOCK_TYPE
	{
		GO_GRASS,
		GO_GLASS,
		//GO_GLASSDAM,
		GO_WOOD,
		//GO_WOODDAM,
		//GO_WOODBROKEN,
		GO_METAL,
		GO_BRICK,
		//GO_BRICKDAM,
		//GO_BRICKBROKEN,
		GO_BTYPTOTAL,
	};
	BLOCK_TYPE Btype;

	enum TOOL_TYPE
	{
		NON_TOOL = 0,
		PICKAXE,
		CANNON,
		DRILL,
		THUMPER,
		MISSILE,
		TOOL_TOTAL,
	};
	TOOL_TYPE tooltype;

	enum TOOL_PROJ
	{
		NON_TOOLPROJ = 0,
		CANNONBALL,
		DRILLPROJ,
		ROCKET,
		TOOLPROJ_TOTAL,
	};
	TOOL_PROJ toolproj;

	GameObject(Grid* grid, GAMEOBJECT_TYPE typeValue = GO_BALL, BLOCK_TYPE Btype_ = GO_GRASS);
	~GameObject();

	bool active;

	// Object Movement
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 dir;
	bool isonAir;
	bool onGround;

	// Object Rotation
	float rotation;
	Vector3 torque;
	Vector3 force;
	float momentOfInertia;
	float angularVelocity;

	// Collision
	AABB aabb;
	float restitution;
	bool iscolliding;
	float mass;
	float invmass;

	//Friction
	float staticFric;
	float dynamicFric;

	// Spatial Partioning
	GameObject* next_;
	GameObject* prev_;
	Grid* m_grid;
	int m_iCurrCellX;
	int m_iCurrCellY;
};

#endif