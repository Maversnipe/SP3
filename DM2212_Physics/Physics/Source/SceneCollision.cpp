#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "SpatialPartitioning\Grid.h"
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

	//RenderMinimap(); //test

	// Spatial Partionining
	m_grid = new Grid();

    //Map reading
    map = new FileIO();
    map->Init(Application::GetWindowHeight() * 4.f, Application::GetWindowWidth() * 4.f, 30, 48, Application::GetWindowHeight() * 1.5f, Application::GetWindowWidth() * 1.5f, 30, 30);
    map->Read("Maps//test.csv");
    RenderMap();
	//RenderMainMinimap();
	
	//bg
	CBackground::GetInstance()->Init();

    //Player
    player = PlayerInfo::GetInstance();
	player->Init(m_grid);

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

    m_ghost = new GameObject(m_grid, GameObject::GO_WALL);
	m_Block = new Block(m_grid);
	m_objectCount = 0;

	initialKE = 0.0f;
	finalKE = 0.0f;

	m_vec3Gravity.Set(0, -9.8, 0);

	//CMinimap::Init
	//cm->SetWorldSize(Application::GetWindowHeight(), Application::GetWindowWidth());

	CollisionManager::getCManager()->SetWorldSize(Application::GetWindowHeight(), Application::GetWindowWidth());
	//CMinimap:: GetInstance()-> Init(Application::GetWindowWidth() / 8, Application::GetWindowHeight()/8);
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

    GameObject *go = new GameObject(m_grid, GameObject::GO_BALL);
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

	Block *go = new Block(m_grid);
	m_vBlocks.push_back(go);

	go->active = true;
	return go;
}

void SceneCollision::Update(double dt)
{
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

	/*if (Application::IsKeyPressed(VK_F10))
	{
		SceneManager::currscene = 3;
	}*/

	//Mouse Section
	//  static bool bLButtonState = false;
	//  if(!bLButtonState && Application::IsMousePressed(0))
	//  {
	//      bLButtonState = true;
	//      std::cout << "LBUTTON DOWN" << std::endl;
	//      
	//      double x, y;
	//      Application::GetCursorPos(&x, &y);
	//      int w = Application::GetWindowWidth();
	//      int h = Application::GetWindowHeight();
	//      float posX = static_cast<float>(x) / w * m_worldWidth + camera.GetOffset_x();
	//      float posY = (h - static_cast<float>(y)) / h * m_worldHeight + camera.GetOffset_y();

	//      m_ghost->pos.Set(posX, posY, 0); //IMPT
	//      float sc = 2;
	//      m_ghost->scale.Set(sc, sc, sc);
	//  }
	//  else if(bLButtonState && !Application::IsMousePressed(0))
	//  {
	//      bLButtonState = false;
	//      std::cout << "LBUTTON UP" << std::endl;

	//      //spawn small GO_BALL
	//      GameObject *go = FetchGO();
	//      go->type = GameObject::GO_EXPLOSION;
	//      double x, y;
	//      Application::GetCursorPos(&x, &y);
	//      int w = Application::GetWindowWidth();
	//      int h = Application::GetWindowHeight();
	//      float posX = static_cast<float>(x) / w * m_worldWidth + camera.GetOffset_x();
	//      float posY = (h - static_cast<float>(y)) / h * m_worldHeight + camera.GetOffset_y();

	//      go->pos = m_ghost->pos;
	//      m_ghost->active = false;
	//      go->scale.Set(1, 1, 1);
	//      go->mass = 3.f;
	//go->aabb.SetAABB(go->pos, go->scale);
	//  }
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

		/*Block *go = FetchGo1();
		go->type = GameObject::GO_BLOCK;
		go->pos = m_ghost->pos;
		go->scale.Set(4.f, 4.f, 1.f);
		go->vel.Set(m_ghost->pos.x - posX, m_ghost->pos.y - posY, 0);
		go->mass = 1.f;
		go->Btype = GameObject::BLOCK_TYPE::GO_WOOD;
		go->aabb.SetAABB(go->pos, go->scale);*/

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
		m_grid->Add(go);
    }

	//Physics Simulation Section
	if (m_timerStarted)
		m_timeTaken1 += dt;

	dt *= m_speed;

	UpdateObjects(dt);
	UpdateBlocks(dt);
	camera.Update(dt);
}

void SceneCollision::RenderMap()
{
	for (int i = 0; i < map->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < map->GetNumOfTiles_Width(); k++)
		{
			if (map->Map[i][k] == 3)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(44.f, 12.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 0.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_GRASS;
				go->Init();
				go->aabb.SetAABB(go->pos, go->scale);
				//m_grid->Add(go);
			}
			else if (map->Map[i][k] == 2)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_GLASS;
				go->Init();
				go->aabb.SetAABB(go->pos, go->scale);
				//m_grid->Add(go);
			}
			else if (map->Map[i][k] == 1)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
                go->Btype = GameObject::BLOCK_TYPE::GO_WOOD;
				go->Init();
				go->aabb.SetAABB(go->pos, go->scale);
				//m_grid->Add(go);
            }
            else if (map->Map[i][k] == 4)
            {
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((float)(k + 1) * 4.f, (float)(map->GetNumOfTiles_Height() - i) * 4.f, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
                go->Btype = GameObject::BLOCK_TYPE::GO_METAL;
				go->Init();
				go->aabb.SetAABB(go->pos, go->scale);
				//m_grid->Add(go);
            }
			else if (map->Map[i][k] == 5)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((float)(k + 1) * 4.f, (float)(map->GetNumOfTiles_Height() - i) * 4.f, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0.f, 0.f, 0);
				go->mass = 1.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_BRICK;
				go->Init();
				go->aabb.SetAABB(go->pos, go->scale);
				//m_grid->Add(go);
			}
		}
	}

	//For debug
	/*
	for (int i = 0; i < map->GetNumOfTiles_Height(); i++)
	{
	for (int k = 0; k < map->GetNumOfTiles_Width(); k++)
	{
	std::cout << map->Map[i][k];
	}

	std::cout << std::endl;
	}
	*/
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

	RenderMainMinimap();
	modelStack.PushMatrix();
	if (CMinimap::GetInstance()->m_cMinimap_Background)
	{
		modelStack.PushMatrix();
		RenderMesh(BGlist[GEO_BONUS], false); //and here
		modelStack.PopMatrix();
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

void SceneCollision::RenderMainMinimap()
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
	for (int i = 0; i < map->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < map->GetNumOfTiles_Width(); k++)
		{
			if (map->Map[i][k] == 3)
			{
				modelStack.PushMatrix();
				modelStack.Scale(0.15, 0.1, 0.05);
				modelStack.Translate(((k + 1)*0.09) - 3.2, ((map->GetNumOfTiles_Height() - i)*0.25)-5.2, 0);
				RenderMesh(BlockList[GEO_GRASS][0], false);
				modelStack.PopMatrix();

			}
			else if (map->Map[i][k] == 2)
			{
				modelStack.PushMatrix();
				modelStack.Scale(0.025, 0.035, 0.05);
				modelStack.Translate(((k + 1)*0.77) - 27.5, ((map->GetNumOfTiles_Height() - i)*0.77) - 14.5, 0);
				RenderMesh(BlockList[GEO_GLASS][0], false);
				modelStack.PopMatrix();
			}
			else if (map->Map[i][k] == 1)
			{
				modelStack.PushMatrix();
				modelStack.Scale(0.025, 0.035, 0.05);
				modelStack.Translate(((k + 1)*0.77) - 27.5, ((map->GetNumOfTiles_Height() - i)*0.77) - 14.5, 0);
				RenderMesh(BlockList[GEO_WOOD][0], false);
				modelStack.PopMatrix();
			}
			else if (map->Map[i][k] == 4)
			{
				modelStack.PushMatrix();
				modelStack.Scale(0.025, 0.035, 0.05);
				modelStack.Translate(((k + 1)*0.77) - 27.5, ((map->GetNumOfTiles_Height() - i)*0.77) - 14.5, 0);
				RenderMesh(BlockList[GEO_METAL][0], false);
				modelStack.PopMatrix();
			}
			else if (map->Map[i][k] == 5)
			{
				modelStack.PushMatrix();
				modelStack.Scale(0.025, 0.035, 0.05);
				modelStack.Translate(((k + 1)*0.77) - 27.5, ((map->GetNumOfTiles_Height() - i)*0.77) - 14.5, 0);
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
	CBackground::GetInstance()->SetBackground(BGlist[GEO_BONUS]); //change bg here

	modelStack.PushMatrix();
	modelStack.Translate

	(CBackground::GetInstance()->getPosition().x,
		CBackground::GetInstance()->getPosition().y,
		CBackground::GetInstance()->getPosition().z);
	modelStack.Scale
	(CBackground::GetInstance()->getScale().x,
		CBackground::GetInstance()->getScale().y,
		CBackground::GetInstance()->getScale().z);

	modelStack.PushMatrix();
	if (CBackground::GetInstance()->m_CBackground)
	{
		modelStack.PushMatrix();
		RenderMesh(BGlist[GEO_BONUS], false); //and here
		modelStack.PopMatrix();
	}

	modelStack.PopMatrix();
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
			//cannonball->Init();
			//cannonball->Update(dt);
			//m_grid->Move(cannonball);
			cannonball->Update(m_goList, m_vBlocks, dt);
		}
		else if (i->toolproj == GameObject::TOOL_PROJ::DRILLPROJ)
		{
			DrillProj* drillproj = static_cast<DrillProj*>(i);
			drillproj->Update(dt);
			m_grid->Move(drillproj);
		}
		if (i->toolproj == GameObject::TOOL_PROJ::ROCKET)
		{
			missile* Missile = static_cast<missile*>(i);

			Missile->Update(mousepos, dt);
			m_grid->Move(Missile);
		}
	}
}

void SceneCollision::UpdateBlocks(double dt)
{
	for (auto &i : m_vBlocks)
	{
		//i->Update(m_goList, m_vBlocks, dt);
		if (!i->active)
			continue;

		if (i->Btype == GameObject::BLOCK_TYPE::GO_GRASS)
		{
			Grassblock* b = static_cast<Grassblock*>(i);

			if (b != NULL)
			{
				//b->Update(dt);
				b->Update(m_goList, m_vBlocks, dt);
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_GLASS)
		{
			Glassblock* b = static_cast<Glassblock*>(i);

			if (b != NULL)
			{
				//b->Update(dt);
				//m_grid->Move(b);
				b->Update(m_goList, m_vBlocks, dt);
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_WOOD)
		{
			Woodblock* b = static_cast<Woodblock*>(i);

			if (b != NULL)
			{
				//b->Update(dt);
				//m_grid->Move(b);
				b->Update(m_goList, m_vBlocks, dt);
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_METAL)
		{
			Metalblock* b = static_cast<Metalblock*>(i);

			if (b != NULL)
			{
				//b->Update(dt);
				//m_grid->Move(b);
				b->Update(m_goList, m_vBlocks, dt);
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_BRICK)
		{
			Brickblock* b = static_cast<Brickblock*>(i);

			if (b != NULL)
			{
				//b->Update(dt);
				//m_grid->Move(b);
				b->Update(m_goList, m_vBlocks, dt);
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
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
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
	case GameObject::GO_TEST_ANIMATION:
		modelStack.PushMatrix();
		modelStack.Translate(50, 50, 0);
		modelStack.Scale(10, 10, 1);
		RenderMesh(meshList[GEO_TEST_ANIMATION], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_BLOCK:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z - 1);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0.f, 0.f, 1.f);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if(go->block_status == GameObject::BLOCK_STATUS::FULL_HEALTH)
			RenderMesh(BlockList[go->Btype][0], false);
		else if (go->block_status == GameObject::BLOCK_STATUS::DAMAGED)
			RenderMesh(BlockList[go->Btype][1], false);
		else if (go->block_status == GameObject::BLOCK_STATUS::BROKEN)
			RenderMesh(BlockList[go->Btype][2], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_EXPLOSION:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_TOOLS:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z - 1);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0.f, 0.f, 1.f);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		RenderMesh(ToolList[go->tooltype], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_CANNON:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z - 1.f);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0.f, 0.f, 1.f);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;
	}
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

	RenderMinimap(); //test

	RenderMesh(meshList[GEO_AXES], false);

	RenderGO(player->GetActiveTool());//render  player active tool

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
	/* while(m_goList.size() > 0)
	{
	GameObject *go = m_goList.back();
	delete go;
	m_goList.pop_back();
	}
	if(m_ghost)
	{
	delete m_ghost;
	m_ghost = NULL;
	}*/

	/*if (m_grid)
	{
	delete m_grid;
	m_grid = NULL;
	}*/
}
