#include "SceneEditor.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "SpatialPartitioning\Grid.h"

SceneEditor::SceneEditor()
{
}

SceneEditor::~SceneEditor()
{
}

void SceneEditor::Init()
{
	SceneBase::Init();
	m_objectCount = 0;
	i_blocklimit = 50;
	//RenderMinimap(); //test

	// Spatial Partionining
	m_grid = new Grid();

	//Map reading
	map = new FileIO();
	map->Init(Application::GetWindowHeight() * 4.f, Application::GetWindowWidth() * 4.f, 40, 64, Application::GetWindowHeight() * 2.f, Application::GetWindowWidth() * 2.f, 30, 30);
	map->Read("Maps//example.csv");
	RenderMap();
	//RenderMainMinimap();

	//Player
	player = PlayerInfo::GetInstance();
	player->Init(m_grid);

	//mapeditor
	mapeditor = MapEditor::GetInstance();
	mapeditor->Init(m_grid);

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();


	m_ghost = new GameObject(m_grid, GameObject::GO_WALL);
	m_Block = new Block(m_grid);

	initialKE = 0.0f;
	finalKE = 0.0f;

	m_vec3Gravity.Set(0, -9.8, 0);

	//CMinimap::Init
	//cm->SetWorldSize(Application::GetWindowHeight(), Application::GetWindowWidth());

	CollisionManager::getCManager()->SetWorldSize(Application::GetWindowHeight(), Application::GetWindowWidth());
	//CMinimap:: GetInstance()-> Init(Application::GetWindowWidth() / 8, Application::GetWindowHeight()/8);
}

GameObject* SceneEditor::FetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			//++m_objectCount;
			return go;
		}
	}

	GameObject *go = new GameObject(m_grid, GameObject::GO_BALL);
	m_goList.push_back(go);


	go->active = true;
	//++m_objectCount;
	return go;
}

Block* SceneEditor::FetchGo1()
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

void SceneEditor::Update(double dt)
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
	Vector3 mousepos(posX, posY, 0);
	SceneBase::Update(dt);
	player->Update(dt, mousepos);//updates player and tools
	mapeditor->Update(dt, mousepos);
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

		if (mapeditor->GetIsEditing())
		{
			if (m_objectCount < i_blocklimit && mapeditor->PlaceBlock(m_vBlocks, m_grid))
			{
				m_objectCount++;
			}
			else if(mapeditor->RemoveBlock(m_vBlocks, m_grid))
			{
				m_objectCount--;
			}
		}
		else
			player->UseCurrentTool(m_vBlocks, m_goList);
	}
	// save file
	static bool isS = false;
	if (Application::IsKeyPressed('S') && !isS)
		isS = true;
	else if (!Application::IsKeyPressed('S') && isS)
	{
		mapeditor->SaveMap(m_vBlocks);
		isS = false;
	}
	static bool isD = false;
	if (Application::IsKeyPressed('D') && !isD)
		isD = true;
	else if (!Application::IsKeyPressed('D') && isD)
	{
		m_objectCount -= mapeditor->DeleteMap(m_vBlocks);
		isD = false;
	}
	static bool isW = false;
	if (Application::IsKeyPressed('W') && !isW)
		isW = true;
	else if (!Application::IsKeyPressed('W') && isW)
	{
		if(mapeditor->GetIsEditing())
			mapeditor->SetIsEditing(false);
		else
			mapeditor->SetIsEditing(true);

		isW = false;
	}
	//Mouse Section
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
		go->toolproj = GameObject::TOOL_PROJ::CANNONBALL;
		go->pos = m_ghost->pos;
		go->vel.Set(m_ghost->pos.x - posX, m_ghost->pos.y - posY, 0);
		m_ghost->active = false;
		float sc = 3.f;
		go->scale.Set(sc, sc, sc);
		go->mass = 3.f;
		go->aabb.SetAABB(go->pos, go->scale);
		//m_grid->Add(go);
	}

	//Physics Simulation Section
	if (m_timerStarted)
		m_timeTaken1 += dt;

	dt *= m_speed;

	UpdateObjects(dt);
	UpdateBlocks(dt);
	camera.Update(dt);
}

void SceneEditor::RenderMap()
{
	for (int i = 0; i < map->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < map->GetNumOfTiles_Width(); k++)
		{
			if (map->Map[i][k] > 0)
			{
				m_objectCount++;
			}
			if (map->Map[i][k] == 1)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_GRASS;
				go->aabb.SetAABB(go->pos, go->scale);
				m_grid->Add(go);
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
				go->aabb.SetAABB(go->pos, go->scale);
				m_grid->Add(go);
			}
			else if (map->Map[i][k] == 3)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_WOOD;
				m_grid->Add(go);
			}
			else if (map->Map[i][k] == 4)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_METAL;
				m_grid->Add(go);
			}
			else if (map->Map[i][k] == 5)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0.f, 0.f, 0);
				go->mass = 1.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_BRICK;
				m_grid->Add(go);
			}
			else if (map->Map[i][k] == 10)
			{
				GameObject *go = FetchGO();
				go->type = GameObject::GO_WALL;
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
				m_grid->Add(go);
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

void SceneEditor::RenderMinimap()
{

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
		RenderMesh(Maplist[GEO_MAPBG], false);
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

void SceneEditor::RenderMainMinimap()
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

				modelStack.Scale(0.04, 0.06, 0.05);
				modelStack.Translate(((k + 1)*0.4) - 10, ((map->GetNumOfTiles_Height() - i) - 30)*0.2, 0);
				RenderMesh(BlockList[GEO_GRASS], false);
				modelStack.PopMatrix();

			}
			else if (map->Map[i][k] == 2)
			{
				modelStack.PushMatrix();

				modelStack.Scale(0.04, 0.06, 0.05);
				modelStack.Translate(((k + 1)*0.4) - 10, ((map->GetNumOfTiles_Height() - i) - 30)*0.2, 0);
				RenderMesh(BlockList[GEO_GLASS], false);
				modelStack.PopMatrix();
			}
			else if (map->Map[i][k] == 1)
			{
				modelStack.PushMatrix();

				modelStack.Scale(0.04, 0.06, 0.05);
				modelStack.Translate(((k + 1)*0.4) - 10, ((map->GetNumOfTiles_Height() - i) - 30)*0.2, 0);
				RenderMesh(BlockList[GEO_WOOD], false);
				modelStack.PopMatrix();
			}
			else if (map->Map[i][k] == 4)
			{
				modelStack.PushMatrix();

				modelStack.Scale(0.04, 0.06, 0.05);
				modelStack.Translate(((k + 1)*0.4) - 10, ((map->GetNumOfTiles_Height() - i) - 30)*0.2, 0);
				RenderMesh(BlockList[GEO_METAL], false);
				modelStack.PopMatrix();
			}
			else if (map->Map[i][k] == 5)
			{
				modelStack.PushMatrix();

				modelStack.Scale(0.04, 0.06, 0.05);
				modelStack.Translate(((k + 1)*0.4) - 10, ((map->GetNumOfTiles_Height() - i) - 30)*0.2, 0);
				RenderMesh(BlockList[GEO_BRICK], false);
				modelStack.PopMatrix();
			}
			/*else if (map->Map[i][k] == 10)
			{
			GameObject *go = FetchGO();
			go->type = GameObject::GO_WALL;
			go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i), 0);
			go->scale.Set(1.f, 1.f, 1.f);
			go->vel.Set(0, 0, 0);
			go->mass = 0.f;
			}*/
		}
	}

	//	modelStack.PopMatrix();
	//modelStack.PopMatrix();
}

void SceneEditor::UpdateObjects(double dt)
{
	for (auto &i : m_goList)
	{
		//i->Update(dt);
		if (i->toolproj == GameObject::TOOL_PROJ::CANNONBALL)
		{
			Cannonball* cannonball = static_cast<Cannonball*>(i);
			cannonball->Update(dt);
			m_grid->Move(cannonball);
		}
		if (i->toolproj == GameObject::TOOL_PROJ::DRILLPROJ)
		{
			DrillProj* drillproj = static_cast<DrillProj*>(i);
			drillproj->Update(dt);
			m_grid->Move(drillproj);
		}
		if (i->toolproj == GameObject::TOOL_PROJ::ROCKET)
		{
			missile* Missile = static_cast<missile*>(i);
			//Missile->Update(mousepos, dt);
			//m_grid->Move(drillproj);
		}
	}
}

void SceneEditor::UpdateBlocks(double dt)
{
	for (auto &i : m_vBlocks)
	{
		//i->Update(m_goList, m_vBlocks, dt);

		if (i->Btype == GameObject::BLOCK_TYPE::GO_GRASS)
		{
			Grassblock* b = static_cast<Grassblock*>(i);

			if (b != NULL)
			{
				b->Update(dt);
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_GLASS)
		{
			Glassblock* b = static_cast<Glassblock*>(i);

			if (b != NULL)
			{
				b->Update(dt);
				m_grid->Move(b);
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_WOOD)
		{
			Woodblock* b = static_cast<Woodblock*>(i);

			if (b != NULL)
			{
				b->Update(dt);
				m_grid->Move(b);
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_METAL)
		{
			Metalblock* b = static_cast<Metalblock*>(i);

			if (b != NULL)
			{
				b->Update(dt);
				m_grid->Move(b);
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_BRICK)
		{
			Brickblock* b = static_cast<Brickblock*>(i);

			if (b != NULL)
			{
				b->Update(dt);
				m_grid->Move(b);
			}
		}
	}
}

void SceneEditor::RenderGO(GameObject *go)
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
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)) + go->rotation, 0.f, 0.f, 1.f);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0.f, 0.f, 1.f);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(BlockList[go->Btype], false);
		modelStack.PopMatrix();
		break;

	case GameObject::GO_TOOLS:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z - 1);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0.f, 0.f, 1.f);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(ToolList[go->tooltype], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneEditor::Render()
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

	if (mapeditor->GetIsEditing())
		RenderGO(mapeditor->GetCurrentBlock());//render  player active tool
	else
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
	ss.str(std::string());
	ss.precision(3);
	ss << "Blocks: " << m_objectCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

	ss.str(std::string());
	ss.precision(5);
	ss << "FPS: " << fps;
	// ss << "minimappos: " << CMinimap::GetInstance()->getPosition();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);

	//RenderMinimap(); //test

}

void SceneEditor::Exit()
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

	if (m_grid)
	{
		delete m_grid;
		m_grid = NULL;
	}
}
