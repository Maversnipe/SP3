#ifndef SCENE_ASTEROID_H
#define SCENE_ASTEROID_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneAsteroid : public SceneBase
{
	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

public:
	SceneAsteroid();
	~SceneAsteroid();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
protected:

	// Physics
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	Vector3 m_force;
	Vector3 m_torque;

	// Asteroids
	std::vector<GameObject *> m_goList;

	// Ship & Player Info
	GameObject *m_ship;
	int m_lives;
	int m_score;

	// Enemy Ship
	/*std::vector<GameObject *> m_enemyShip;*/
};

#endif