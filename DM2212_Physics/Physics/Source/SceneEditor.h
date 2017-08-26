#ifndef SCENE_EDITOR_H
#define SCENE_EDITOR_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "FileIO.h"
#include "PlayerInfo.h"
#include "Blocks.h"
#include "GrassBlock.h"
#include "GlassBlock.h"
#include "WoodBlock.h"
#include "MetalBlock.h"
#include "BrickBlock.h"
#include "Cannonball.h"
#include "DrillProj.h"
#include "missile.h"
#include "CollisionManager.h"
#include "minimap.h"
#include "MapEditor.h"

class SceneEditor : public SceneBase
{

public:
	SceneEditor();
	~SceneEditor();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
	Block* FetchGo1();

	//Mapping
	void RenderMap();

	//render minimap
	void RenderMinimap();
	void RenderMainMinimap();

protected:
	//Physics
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;
	float m_timeEstimated1;
	float m_timeTaken1;
	bool m_timerStarted;
	int i_blocklimit;

	//Objects
	GameObject *m_ghost;
	std::vector<GameObject *> m_goList;
	void UpdateObjects(double dt);

	std::vector<Block *> m_vBlocks;
	Block *m_Block;
	void UpdateBlocks(double dt);

	//"Gravity"
	Vector3 m_vec3Gravity;

	//Minimap
	Vector3 minimapvector;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
	Vector3 initialMomentum, finalMomentum;
	float initialKE, finalKE;

	//Map
	FileIO* map;

	//player
	PlayerInfo* player;

	//Mapeditor
	MapEditor* mapeditor;

	// Spatial Partioning
	Grid* m_grid;
};

#endif