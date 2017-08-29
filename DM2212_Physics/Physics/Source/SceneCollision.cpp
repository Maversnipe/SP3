#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "SceneManager.h"

SceneCollision::SceneCollision()
{
}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
{
	SceneBase::Init();

	StateManager::getInstance()->Init();
	timer = 0.0f;
	pause = false;

	Map = new FileIO*[4];
	Map[0] = new FileIO();
	Map[1] = new FileIO();
	Map[2] = new FileIO();
	Map[3] = new FileIO();
	InitMap(1);

	m_iCurrMap = 1;
	
	//bg
	CBackground::GetInstance()->Init();

    //Player
    player = PlayerInfo::GetInstance();
	player->Init();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

    m_ghost = new GameObject(GameObject::GO_WALL);
	m_Block = new Block();
	m_objectCount = 0;
	m_iBlockCount = 0;

	LEVEL_ONE_TARGET = 500;
	LEVEL_TWO_TARGET = 500;
	LEVEL_THREE_TARGET = 500;
	LEVEL_FOUR_TARGET = 500;
	//CMinimap::Init
	//cm->SetWorldSize(Application::GetWindowHeight(), Application::GetWindowWidth());

	CollisionManager::getCManager()->SetWorldSize(Application::GetWindowHeight(), Application::GetWindowWidth());
	//CMinimap:: GetInstance()-> Init(Application::GetWindowWidth() / 8, Application::GetWindowHeight()/8);

	//CSoundEngine::GetInstance()->PlayASound("test");
}

void SceneCollision::InitMap(int mapNum)
{
	//Map reading
	// Level 1
	if (mapNum == 1)
	{
		m_iBlockCount = 0;
		Map[0]->Init(Application::GetWindowHeight() * 4.f, Application::GetWindowWidth() * 4.f, 30, 48, Application::GetWindowHeight() * 1.5f, Application::GetWindowWidth() * 1.5f, 30, 30);
		Map[0]->Read("Maps//Level_01.csv");
		RenderMap(0);
	}
	else if (mapNum == 2)
	{
		// Level 2
		m_iBlockCount = 0;
		Map[1]->Init(Application::GetWindowHeight() * 4.f, Application::GetWindowWidth() * 4.f, 30, 48, Application::GetWindowHeight() * 1.5f, Application::GetWindowWidth() * 1.5f, 30, 30);
		Map[1]->Read("Maps//Level_02.csv");
		RenderMap(1);
	}
	else if (mapNum == 3)
	{
		// Level 3
		m_iBlockCount = 0;
		Map[2]->Init(Application::GetWindowHeight() * 4.f, Application::GetWindowWidth() * 4.f, 30, 48, Application::GetWindowHeight() * 1.5f, Application::GetWindowWidth() * 1.5f, 30, 30);
		Map[2]->Read("Maps//Level_03.csv");
		RenderMap(2);
	}
	else if (mapNum == 4)
	{
		// Level 4
		m_iBlockCount = 0;
		Map[3]->Init(Application::GetWindowHeight() * 4.f, Application::GetWindowWidth() * 4.f, 30, 48, Application::GetWindowHeight() * 1.5f, Application::GetWindowWidth() * 1.5f, 30, 30);
		Map[3]->Read("Maps//Level_04.csv");
		RenderMap(3);
	}
}

void SceneCollision::LevelManager()
{
	if(m_iCurrMap < 4)
		++m_iCurrMap;

	if (m_iCurrMap > 4)
	{
		// Win Game screen
	}
	else
	{
		// Loading screen
		for (std::vector<Block *>::iterator it = m_vBlocks.begin(); it != m_vBlocks.end(); ++it)
		{
			Block *go = (Block *)*it;
			if (go->active)
			{
				go->active = false;
			}
		}
		InitMap(m_iCurrMap);
	}
}

GameObject* SceneCollision::FetchGO()
{
    for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject *go = (GameObject *)*it;
        if(!go->active)
        {
            go->active = true;
            ++m_objectCount;
            return go;
        }
    }

    GameObject *go = new GameObject(GameObject::GO_BALL);
    m_goList.push_back(go);

   
    go->active = true;
    ++m_objectCount;
    return go;
}

Block* SceneCollision::FetchGo1()
{
	for (std::vector<Block *>::iterator it = m_vBlocks.begin(); it != m_vBlocks.end(); ++it)
	{
		Block *go = (Block *)*it;
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}

	Block *go = new Block();
	m_vBlocks.push_back(go);

	go->active = true;
	return go;
}

void SceneCollision::Update(double dt)
{
	StateManager::getInstance()->Update(dt);
	
	if (StateManager::getInstance()->GetState() != S_PLAYING)
		return;

	timer += dt;

	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float posX = static_cast<float>(x) / w * m_worldWidth + camera.GetOffset_x();
	float posY = (h - static_cast<float>(y)) / h * m_worldHeight + camera.GetOffset_y();
	static bool isFullScreen = false;
	int offsetWindowX = Application::GetWindowWidth() / 8;
	int offsetWindowY = Application::GetWindowHeight() / 8;
	int offsetX = 90;
	mousepos.Set(posX, posY, 0);
	SceneBase::Update(dt);


	static bool bSpaceState = false;
	if (!bSpaceState && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;
		std::cout << "SPACE BAR DOWN" << std::endl;
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
		std::cout << "SPACE BAR UP" << std::endl;

		player->UseCurrentTool(m_vBlocks, m_goList);
	}

	player->Update(dt, mousepos);//updates player and tools

	//fullscreen and default screensize for minimap position
	if (Application::GetWindowWidth() / 8 != 120)
	{
		isFullScreen = true;
	}
	else
	{
		isFullScreen = false;
	}
	if (!isFullScreen)
	{
		CMinimap::GetInstance()->Init(offsetWindowX, 75);
	}
	else
	{
		CMinimap::GetInstance()->Init(offsetWindowX - offsetX, 75);
	}

	if (Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		m_ghost->pos.Set(posX, posY, 0); //IMPT
		m_ghost->active = true;
		float sc = 3;
		m_ghost->scale.Set(sc, sc, sc);
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		GameObject *go = FetchGO();
		go->type = GameObject::GO_BALL;
		go->toolproj = GameObject::CANNONBALL;
		go->pos = m_ghost->pos;
		go->vel.Set(m_ghost->pos.x - posX, m_ghost->pos.y - posY, 0);
		m_ghost->active = false;
		float sc = 3.f;
		go->scale.Set(sc, sc, sc);
		go->mass = 5.f;
		go->aabb.SetAABB(go->pos, go->scale);
    }

	//Physics Simulation Section
	if (m_timerStarted)
		m_timeTaken1 += dt;

	dt *= m_speed;

	static bool F = false;
	if (Application::IsKeyPressed('F') && F == false)
	{
		F = true;
	}
	if (Application::IsKeyPressed('F') && F == true)
	{
		LevelManager();
		F = false;
	}

	UpdateObjects(dt);
	UpdateBlocks(dt);
	camera.Update(dt);

	if (m_iBlockCount <= 0)
	{ // Lose
		if (m_iCurrMap == 1)
		{
			if (player->GetGold() < LEVEL_ONE_TARGET)
			{
				// Gameover
			}
		}
		else if (m_iCurrMap == 2)
		{
			if (player->GetGold() < LEVEL_TWO_TARGET)
			{
				// Gameover
			}
		}
		else if (m_iCurrMap == 3)
		{
			if (player->GetGold() < LEVEL_THREE_TARGET)
			{
				// Gameover
			}
		}
		else if (m_iCurrMap == 4)
		{
			if (player->GetGold() < LEVEL_FOUR_TARGET)
			{
				// Gameover
			}
		}
	}
	else
	{
		if (m_iCurrMap == 1)
		{
			if (player->GetGold() > LEVEL_ONE_TARGET)
			{
				LevelManager();
			}
		}
		else if (m_iCurrMap == 2)
		{
			if (player->GetGold() > LEVEL_TWO_TARGET)
			{
				LevelManager();
			}
		}
		else if (m_iCurrMap == 3)
		{
			if (player->GetGold() > LEVEL_THREE_TARGET)
			{
				LevelManager();
			}
		}
		else if (m_iCurrMap == 4)
		{
			if (player->GetGold() > LEVEL_FOUR_TARGET)
			{
				LevelManager();
			}
		}
	}
}

void SceneCollision::RenderMap(int mapNum)
{
	for (int i = 0; i < Map[mapNum]->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < Map[mapNum]->GetNumOfTiles_Width(); k++)
		{
			if (Map[mapNum]->Map[i][k] == 3)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((k + 1) * 4, (Map[mapNum]->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(44.f, 12.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 0.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_GRASS;
				go->Init();
				go->aabb.SetAABB(go->pos, go->scale);
				m_iBlockCount++;
			}
			else if (Map[mapNum]->Map[i][k] == 2)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((k + 1) * 4, (Map[mapNum]->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_GLASS;
				go->Init();
				go->aabb.SetAABB(go->pos, go->scale);
				m_iBlockCount++;
			}
			else if (Map[mapNum]->Map[i][k] == 1)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((k + 1) * 4, (Map[mapNum]->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_WOOD;
				go->Init();
				go->aabb.SetAABB(go->pos, go->scale);
				m_iBlockCount++;
			}
            else if (Map[mapNum]->Map[i][k] == 4)
            {
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((float)(k + 1) * 4.f, (float)(Map[mapNum]->GetNumOfTiles_Height() - i) * 4.f, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_METAL;
				go->Init();
				go->aabb.SetAABB(go->pos, go->scale);
				m_iBlockCount++;
            }
			else if (Map[mapNum]->Map[i][k] == 5)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((float)(k + 1) * 4.f, (float)(Map[mapNum]->GetNumOfTiles_Height() - i) * 4.f, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0.f, 0.f, 0);
				go->mass = 1.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_BRICK;
				go->Init();
				go->aabb.SetAABB(go->pos, go->scale);
				m_iBlockCount++;
			}
			else
				continue;
		}
	}

	//For debug
	
	for (int i = 0; i < Map[mapNum]->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < Map[mapNum]->GetNumOfTiles_Width(); k++)
		{
			std::cout << Map[mapNum]->Map[i][k];
		}

		std::cout << std::endl;
	}
}

void SceneCollision::RenderMinimap()
{
	CMinimap::GetInstance()->SetBackground(BGlist[GEO_BONUS]); //change bg of minimap here

	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
	modelStack.Translate(camera.GetOffset_x() + CMinimap::GetInstance()->getScale().x / 2, camera.GetOffset_y() + CMinimap::GetInstance()->getScale().y / 2, 10);
	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
	// Translate the current transformation (from minimap.cpp)
	modelStack.Translate(CMinimap::GetInstance()->getPosition().x, CMinimap::GetInstance()->getPosition().y, CMinimap::GetInstance()->getPosition().z);
	//modelStack.Translate(CMinimap::GetInstance()->getPosition().x, CMinimap::GetInstance()->getPosition().y, CMinimap::GetInstance()->getPosition().z);
	// Scale the current transformation (from minimap.cpp)
	modelStack.Scale(CMinimap::GetInstance()->getScale().x, CMinimap::GetInstance()->getScale().y, CMinimap::GetInstance()->getScale().z);

	RenderMainMinimap(m_iCurrMap - 1);
	modelStack.PushMatrix();
	if (CMinimap::GetInstance()->m_cMinimap_Background)
	{
		if (m_iCurrMap == 1)
		{
			modelStack.PushMatrix();
			RenderMesh(BGlist[GEO_MORNING], false); //and here
			modelStack.PopMatrix();
		}
		else if (m_iCurrMap == 2)
		{
			modelStack.PushMatrix();
			RenderMesh(BGlist[GEO_MIDDAY], false); //and here
			modelStack.PopMatrix();
		}
		else if (m_iCurrMap == 3)
		{
			modelStack.PushMatrix();
			RenderMesh(BGlist[GEO_EVENING], false); //and here
			modelStack.PopMatrix();
		}
		else if (m_iCurrMap == 4)
		{
			modelStack.PushMatrix();
			RenderMesh(BGlist[GEO_DUSK], false); //and here
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	if (CMinimap::GetInstance()->m_cMinimap_Border)
	{
		modelStack.PushMatrix();
		modelStack.Scale(1.02, 1.02, 1.02);
		RenderMesh(Maplist[GEO_MAPBORDER], false);
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneCollision::RenderMainMinimap(int mapNum)
{
	/*
	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
	modelStack.Translate(camera.GetOffset_x() + CMinimap::GetInstance()->getScale().x / 2, camera.GetOffset_y() + CMinimap::GetInstance()->getScale().y / 2, 10);

	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
	// Translate the current transformation (from minimap.cpp)
	modelStack.Translate(CMinimap::GetInstance()->getPosition().x, CMinimap::GetInstance()->getPosition().y, CMinimap::GetInstance()->getPosition().z);
	*/
	for (int i = 0; i < Map[mapNum]->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < Map[mapNum]->GetNumOfTiles_Width(); k++)
		{
			if (Map[mapNum]->Map[i][k] == 3)
			{
				modelStack.PushMatrix();
				modelStack.Scale(0.15, 0.1, 0.05);
				modelStack.Translate(((k + 1)*0.09) - 3.2, ((Map[mapNum]->GetNumOfTiles_Height() - i)*0.25)-5.2, 0);
				RenderMesh(BlockList[GEO_GRASS][0], false);
				modelStack.PopMatrix();

			}
			else if (Map[mapNum]->Map[i][k] == 2)
			{
				modelStack.PushMatrix();
				modelStack.Scale(0.025, 0.035, 0.05);
				modelStack.Translate(((k + 1)*0.77) - 27.5, ((Map[mapNum]->GetNumOfTiles_Height() - i)*0.77) - 14.5, 0);
				RenderMesh(BlockList[GEO_GLASS][0], false);
				modelStack.PopMatrix();
			}
			else if (Map[mapNum]->Map[i][k] == 1)
			{
				modelStack.PushMatrix();
				modelStack.Scale(0.025, 0.035, 0.05);
				modelStack.Translate(((k + 1)*0.77) - 27.5, ((Map[mapNum]->GetNumOfTiles_Height() - i)*0.77) - 14.5, 0);
				RenderMesh(BlockList[GEO_WOOD][0], false);
				modelStack.PopMatrix();
			}
			else if (Map[mapNum]->Map[i][k] == 4)
			{
				modelStack.PushMatrix();
				modelStack.Scale(0.025, 0.035, 0.05);
				modelStack.Translate(((k + 1)*0.77) - 27.5, ((Map[mapNum]->GetNumOfTiles_Height() - i)*0.77) - 14.5, 0);
				RenderMesh(BlockList[GEO_METAL][0], false);
				modelStack.PopMatrix();
			}
			else if (Map[mapNum]->Map[i][k] == 5)
			{
				modelStack.PushMatrix();
				modelStack.Scale(0.025, 0.035, 0.05);
				modelStack.Translate(((k + 1)*0.77) - 27.5, ((Map[mapNum]->GetNumOfTiles_Height() - i)*0.77) - 14.5, 0);
				RenderMesh(BlockList[GEO_BRICK][0], false);
				modelStack.PopMatrix();
			}
		}
	}

	//	modelStack.PopMatrix();
	//modelStack.PopMatrix();
}

void SceneCollision::RenderBG()
{
	if (m_iCurrMap == 1)
	{
		CBackground::GetInstance()->SetBackground(BGlist[GEO_MORNING]); //change bg here
		if (CBackground::GetInstance()->m_CBackground)
		{
			modelStack.PushMatrix();
			modelStack.Translate(130, 95, -4.f);
			modelStack.Scale(256.0f, 196.0f, 1.f);
			RenderMesh(BGlist[GEO_MORNING], false); //and here
			modelStack.PopMatrix();
		}
	}
	else if (m_iCurrMap == 2)
	{
		CBackground::GetInstance()->SetBackground(BGlist[GEO_MIDDAY]); //change bg here
		if (CBackground::GetInstance()->m_CBackground)
		{
			modelStack.PushMatrix();
			modelStack.Translate(130, 95, -4.f);
			modelStack.Scale(256.0f, 196.0f, 1.f);
			RenderMesh(BGlist[GEO_MIDDAY], false); //and here
			modelStack.PopMatrix();
		}
	}
	else if (m_iCurrMap == 3)
	{
		CBackground::GetInstance()->SetBackground(BGlist[GEO_EVENING]); //change bg here
		if (CBackground::GetInstance()->m_CBackground)
		{
			modelStack.PushMatrix();
			modelStack.Translate(130, 95, -4.f);
			modelStack.Scale(256.0f, 196.0f, 1.f);
			RenderMesh(BGlist[GEO_EVENING], false); //and here
			modelStack.PopMatrix();
		}
	}
	else if (m_iCurrMap == 4)
	{
		CBackground::GetInstance()->SetBackground(BGlist[GEO_DUSK]); //change bg here
		if (CBackground::GetInstance()->m_CBackground)
		{
			modelStack.PushMatrix();
			modelStack.Translate(130, 95, -4.f);
			modelStack.Scale(256.0f, 196.0f, 1.f);
			RenderMesh(BGlist[GEO_DUSK], false); //and here
			modelStack.PopMatrix();
		}
	}
}

void SceneCollision::UpdateObjects(double dt)
{
	for (auto &i : m_goList)
	{
		//i->Update(dt);
		if (!i->active)
			continue;
		if (i->toolproj == GameObject::TOOL_PROJ::CANNONBALL)
		{
			Cannonball* cannonball = static_cast<Cannonball*>(i);
			cannonball->Update(m_goList, m_vBlocks, dt);
		}
		else if (i->toolproj == GameObject::TOOL_PROJ::DRILLPROJ)
		{
			DrillProj* drillproj = static_cast<DrillProj*>(i);
			drillproj->Update(m_goList, m_vBlocks, dt);
		}
		else  if (i->toolproj == GameObject::TOOL_PROJ::ROCKET)
		{
			missile* Missile = static_cast<missile*>(i);

			Missile->Update(m_goList, m_vBlocks, mousepos, dt);
		}
		else  if (i->toolproj == GameObject::TOOL_PROJ::EXPLOSION)
		{
			Explosion* explosive = static_cast<Explosion*>(i);

			explosive->Update(m_goList, m_vBlocks, dt);
		}
	}
}

void SceneCollision::UpdateBlocks(double dt)
{
	for (auto &i : m_vBlocks)
	{
		if (!i->active)
			continue;

		if (i->Btype == GameObject::BLOCK_TYPE::GO_GRASS)
		{
			Grassblock* b = static_cast<Grassblock*>(i);

			if (b != NULL)
			{
				b->Update(m_goList, m_vBlocks, dt);
				if (b->Isdestroyed())
					--m_iBlockCount;
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_GLASS)
		{
			Glassblock* b = static_cast<Glassblock*>(i);

			if (b != NULL)
			{
				b->Update(m_goList, m_vBlocks, dt);
				if (b->Isdestroyed())
					--m_iBlockCount;
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_WOOD)
		{
			Woodblock* b = static_cast<Woodblock*>(i);

			if (b != NULL)
			{
				b->Update(m_goList, m_vBlocks, dt);
				if (b->Isdestroyed())
					--m_iBlockCount;
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_METAL)
		{
			Metalblock* b = static_cast<Metalblock*>(i);

			if (b != NULL)
			{
				b->Update(m_goList, m_vBlocks, dt);
				if (b->Isdestroyed())
					--m_iBlockCount;
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_BRICK)
		{
			Brickblock* b = static_cast<Brickblock*>(i);

			if (b != NULL)
			{
				b->Update(m_goList, m_vBlocks, dt);
				if (b->Isdestroyed())
					--m_iBlockCount;
			}
		}
	}
}

void SceneCollision::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_BALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, -1.f);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0.f, 0.f, 1.f);
		if (go->toolproj == GameObject::TOOL_PROJ::CANNONBALL)
		{
			modelStack.Scale(5.f, 5.f, 5.f);
			RenderMesh(Projectile[GEO_CANNONBALL], false);
		}
		else if (go->toolproj == GameObject::TOOL_PROJ::DRILLPROJ)
		{
			modelStack.Scale(7.5f, 7.5f, 7.5f);
			RenderMesh(ToolList[GEO_DRILL], false);
		}
		else if (go->toolproj == GameObject::TOOL_PROJ::ROCKET)
		{
			modelStack.Scale(7.5f, 7.5f, 7.5f);
			RenderMesh(Projectile[GEO_ROCKET], false);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_WALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0.f, 0.f, 1.f);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PILLAR:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;
		//GAME
	case GameObject::GO_EXPLOSION:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(10, 10, 1);
		RenderMesh(Projectile[GEO_EXPLOSION], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_BLOCK:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if(go->block_status == GameObject::BLOCK_STATUS::FULL_HEALTH)
			RenderMesh(BlockList[go->Btype][0], false);
		else if (go->block_status == GameObject::BLOCK_STATUS::DAMAGED)
			RenderMesh(BlockList[go->Btype][1], false);
		else if (go->block_status == GameObject::BLOCK_STATUS::BROKEN)
			RenderMesh(BlockList[go->Btype][2], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_TOOLS:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 1);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0.f, 0.f, 1.f);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(ToolList[go->tooltype], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneCollision::RenderUI(GameObject * thing)
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.GetOffset_x() + CMinimap::GetInstance()->getScale().x / 2, camera.GetOffset_y() + CMinimap::GetInstance()->getScale().y / 2, 10);
	switch (thing->tooltype)
	{
	case 1:
		modelStack.PushMatrix();
		modelStack.Translate(15, 80, 1);
		modelStack.Scale(56, 11, 1);
		RenderMesh(Toolboxlist[GEO_sPICKAXE], false);
		modelStack.PopMatrix();

	case 2:
		modelStack.PushMatrix();
		modelStack.Translate(15, 80, 1);
		modelStack.Scale(56, 11, 1);
		RenderMesh(Toolboxlist[GEO_sCANNON], false);
		modelStack.PopMatrix();

	case 3:
		modelStack.PushMatrix();
		modelStack.Translate(15, 80, 1);
		modelStack.Scale(56, 11, 1);
		RenderMesh(Toolboxlist[GEO_sDRILL], false);
		modelStack.PopMatrix();

	case 4:
		modelStack.PushMatrix();
		modelStack.Translate(15, 80, 1);
		modelStack.Scale(56, 11, 1);
		RenderMesh(Toolboxlist[GEO_sTHUMPER], false);
		modelStack.PopMatrix();

	case 5:
		modelStack.PushMatrix();
		modelStack.Translate(15, 80, 1);
		modelStack.Scale(56, 11, 1);
		RenderMesh(Toolboxlist[GEO_sMISSILE], false);
		modelStack.PopMatrix();

	case 6:
		modelStack.PushMatrix();
		modelStack.Translate(15, 80, 1);
		modelStack.Scale(56, 11, 1);
		RenderMesh(Toolboxlist[GEO_sDYNAMITE], false);
		modelStack.PopMatrix();

	}
	modelStack.PopMatrix();
	//thing->tooltype;
}

void SceneCollision::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	if (StateManager::getInstance()->GetState() != S_PLAYING)
	{
		StateManager::getInstance()->Render();
		return;
	}

	RenderBG();

	RenderMinimap(); //test

	RenderMesh(meshList[GEO_AXES], false);


	RenderUI(player->GetActiveTool());//render player active tool to change UI

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (go->pos.x > (camera.position.x - 2.f) && go->pos.x < camera.position.x + Application::GetWindowWidth()
				&& go->pos.y >(camera.position.y - 2.f) && go->pos.y < camera.position.y + Application::GetWindowHeight())
				RenderGO(go); // Only render if object is on screen
		}
	}
	for (std::vector<Block *>::iterator it = m_vBlocks.begin(); it != m_vBlocks.end(); ++it)
	{
		Block *go = (Block *)*it;
		if (go->active)
		{
			if (go->pos.x >(camera.position.x - 2.f) && go->pos.x < camera.position.x + Application::GetWindowWidth()
				&& go->pos.y >(camera.position.y - 2.f) && go->pos.y < camera.position.y + Application::GetWindowHeight())
				RenderGO(go); // Only render if object is on screen

		}
	}

	RenderGO(player->GetActiveTool());//render player active tool

	if (m_ghost->active)
		RenderGO(m_ghost);

	//On screen text
	std::ostringstream ss;
	//ss << "Time Estimated: " << m_timeEstimated1;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 18);

	//ss.str(std::string());
	//ss.precision(5);
	//ss << "Time Taken: " << m_timeTaken1;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);

	//ss.str(std::string());
	//ss.precision(5);
	//ss << "E1: " << initialKE;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);
	//
	//ss.str(std::string());
	//ss.precision(5);
	//ss << "E2: " << finalKE;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

	////Exercise 3: render initial and final kinetic energy
	//
	//ss.str(std::string());
	//ss.precision(3);
	//ss << "Speed: " << m_speed;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);
	RenderBG();

	ss.str(std::string());
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	ss.str("");
	ss.precision(5);
	ss << "Time: " << timer;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 46, 3);

	ss.str("");
	ss.precision(5);
	ss << "Money: " << PlayerInfo::GetInstance()->GetGold();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 0);

	if (PlayerInfo::GetInstance()->GetActiveToolIndex() == 1)
	{
		std::ostringstream ss;
		ss.str(std::string());
		ss.precision(5);
		ss << PlayerInfo::GetInstance()->GetString();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 46, 0);
	}
}

void SceneCollision::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	/*while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		if(go!= NULL)
			delete go;
		m_goList.pop_back();
	}*/

	if (m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}

	/*if (m_grid)
	{
	delete m_grid;
	m_grid = NULL;
	}*/
}
