#include "SceneEditor.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "SpatialPartitioning\Grid.h"
#include "background.h"

SceneEditor::SceneEditor()
{
}

SceneEditor::~SceneEditor()
{
}

void SceneEditor::Init()
{
	SceneBase::Init();
	ispickaxe = true;
	iscannon = true;
	isthumper = true;
	isdrill = true;
	ismissile = true;
	isdynamite = true;
	m_objectCount = 0;
	i_blocklimit = 50;
	backgroundindex = 1;
	//RenderMinimap(); //test
	optionsmenu = false;
	// Spatial Partionining
	m_grid = new Grid();

	//Map reading
	map = new FileIO();
    map->Init(Application::GetWindowHeight() * 4.f, Application::GetWindowWidth() * 4.f, 30, 48, Application::GetWindowHeight() * 1.5f, Application::GetWindowWidth() * 1.5f, 30, 30);
	map->Read("Maps//example.csv");
	RenderMap();
	CBackground::GetInstance()->Init();
	//RenderMainMinimap();

	//Player
	player = PlayerInfo::GetInstance();
	//player->Init(m_grid);
	//player->Init(m_grid,0,1,0,1,0,0);// limiting the player items

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

	for (int i = Button::EDITOR_BACKGROUND_LEFT; i < Button::BUTTON_MAX; ++i)//init buttons
	{
		ButtArray[i - Button::EDITOR_BACKGROUND_LEFT] = new Button(Vector3(0, 0, 0), Vector3(1, 1, 1), static_cast<Button::BUTTON_TYPE>(i));
	}
}

GameObject* SceneEditor::FetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}

	GameObject *go = new GameObject(m_grid, GameObject::GO_BALL);
	m_goList.push_back(go);


	go->active = true;
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
	mousepos = Vector3(posX, posY, 0);
	SceneBase::Update(dt);
	if(!mapeditor->GetIsEditing())
	player->Update(dt, mousepos);//updates player and tools
	mapeditor->Update(dt, mousepos);
	UpdateOptions();


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

		if (mapeditor->GetIsEditing())
		{
			if (m_objectCount < i_blocklimit && mapeditor->PlaceBlock(m_vBlocks, m_grid))
			{
				m_objectCount++;
			}
			else if (mapeditor->RemoveBlock(m_vBlocks, m_grid))
			{
				m_objectCount--;
			}
		}
		else
			player->UseCurrentTool(m_vBlocks, m_goList);
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
		std::cout << "SPACE BAR UP" << std::endl;
	}
	// save file
	static bool isS = false;
	if (Application::IsKeyPressed('S') && !isS)
	{
		isS = true;
		if (!optionsmenu && mapeditor->GetIsEditing())
		{
			mapeditor->SaveMap(m_vBlocks);
			optionsmenu = true;
		}
		else
			optionsmenu = false;
	}
	else if (!Application::IsKeyPressed('S') && isS)
	{
		isS = false;
	}

	static bool isD = false;
	if (Application::IsKeyPressed('D') && !isD)
		isD = true;
	else if (!Application::IsKeyPressed('D') && isD)
	{
		if (mapeditor->GetIsEditing())
		{
			m_objectCount -= mapeditor->DeleteMap(m_vBlocks);
		}
		isD = false;
	}
	static bool isEnter = false;
	if (Application::IsKeyPressed(VK_RETURN) && !isEnter)
	{
		isEnter = true;
		if (optionsmenu && mapeditor->GetIsEditing())
		{
			player->Init(m_grid, ispickaxe, iscannon, isthumper, isdrill, isdynamite, ismissile);
			optionsmenu = false;
			if (mapeditor->GetIsEditing())
			{
				mapeditor->SetIsEditing(false);
			}
			
		}
		else
		{
			mapeditor->SetIsEditing(true);//return to editing mode
			m_objectCount -= mapeditor->DeleteMap(m_vBlocks);
			for (int i = 0; i < m_goList.size(); ++i)
			{
				m_goList[i]->active = false;
			}
			RenderMap();
		}
	}
	else if (!Application::IsKeyPressed(VK_RETURN) && isEnter)
	{
		isEnter = false;
	}

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;

		if (optionsmenu)
		{
			for (unsigned i = 0; i < numButtons; ++i)
			{
				if (ButtArray[i]->MouseCheck(mousepos))
				{
					switch (ButtArray[i]->buttype)
					{
					case(Button::EDITOR_BACKGROUND_LEFT):
						backgroundindex--;
						if (backgroundindex < 0)
							backgroundindex = 4;
						break;
					case Button::EDITOR_BACKGROUND_RIGHT:
						backgroundindex++;
						if (backgroundindex > 4)
							backgroundindex = 0;
						break;
					case(Button::EDITOR_MONEY_LEFT):
						if(player->GetGold() > 10)
						player->SetGold(player->GetGold() - 10);
						break;
					case Button::EDITOR_MONEY_RIGHT:
						if(player->GetGold() < 990)
						player->SetGold(player->GetGold() + 10);
						break;
					case(Button::EDITOR_PICKAXE):
						if (ispickaxe)
							ispickaxe = false;
						else
							ispickaxe = true;
						break;
					case Button::EDITOR_CANNON:
						if (iscannon)
							iscannon = false;
						else
							iscannon = true;
						break;
					case(Button::EDITOR_THUMPER):
						if (isthumper)
							isthumper = false;
						else
							isthumper = true;
						break;
					case Button::EDITOR_DRILL:
						if (isdrill)
							isdrill = false;
						else
							isdrill = true;
						break;
					case(Button::EDITOR_MISSILE):
						if (ismissile)
							ismissile = false;
						else
							ismissile = true;
						break;
					case Button::EDITOR_DYNAMITE:
						if (isdynamite)
							isdynamite = false;
						else
							isdynamite = true;
						break;

					}

				}
			}
		}
	}

	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;

	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;

	}
	if(!optionsmenu)
	{
	//Physics Simulation Section
	if (m_timerStarted)
		m_timeTaken1 += dt;

	//dt *= m_speed;

	if (!mapeditor->GetIsEditing())
	{
		UpdateObjects(dt);
		UpdateBlocks(dt);
	}
	if(!optionsmenu)
	camera.Update(dt);
	}
}

void SceneEditor::RenderMap()
{
	for (int i = 0; i < map->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < map->GetNumOfTiles_Width(); k++)
		{
			if (map->Map[i][k] > 1)
			{
				m_objectCount++;
			}
			if (map->Map[i][k] == 1)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(44.f, 12.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
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
			else if (map->Map[i][k] == 3)
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
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
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
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0.f, 0.f, 0);
				go->mass = 1.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_BRICK;
				go->Init();
				go->aabb.SetAABB(go->pos, go->scale);
				//m_grid->Add(go);
			}
			else if (map->Map[i][k] == 10)
			{
				GameObject *go = FetchGO();
				go->type = GameObject::GO_WALL;
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
				go->aabb.SetAABB(go->pos, go->scale);
				//m_grid->Add(go);
			}
		}
	}
}

void SceneEditor::RenderBG()
{
	CBackground::GetInstance()->SetBackground(BGlist[backgroundindex]); //change bg here
	if (CBackground::GetInstance()->m_CBackground)
	{
		modelStack.PushMatrix();
		modelStack.Translate(130, 95, -4.f);
		modelStack.Scale(256.0f, 196.0f, 1.f);
		RenderMesh(BGlist[backgroundindex], false); //and here
		modelStack.PopMatrix();
	}
}

void SceneEditor::RenderOptions()
{
	Vector3 center(m_worldWidth * 0.5 + 2 + camera.GetOffset_x(), m_worldHeight* 0.5 + 6 + camera.GetOffset_y(), 0);

	modelStack.PushMatrix();
	modelStack.Translate(center.x, center.y, 2);
	modelStack.Scale(m_worldWidth * 0.8, m_worldHeight*0.8, 1.f);
	RenderMesh(meshList[GEO_TEXTBOX], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(center.x, center.y -20, 3);
	modelStack.Scale(56, 10.6, 1.f);
	RenderMesh(meshList[GEO_ITEMSELECT], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();//SceneBackground
	modelStack.Translate(center.x-30, center.y-10, 3);
	modelStack.Scale(5, 5, 1.f);
	RenderText(meshList[GEO_TEXT], "Player weapons", Color(0, 0, 0));
	modelStack.PopMatrix();

	modelStack.PushMatrix();//SceneBackground
	modelStack.Translate(center.x - 30, center.y + 10, 3);
	modelStack.Scale(10, 10, 1.f);
	RenderText(meshList[GEO_TEXT], std::to_string(backgroundindex), Color(0, 0, 0));
	modelStack.PopMatrix();

	modelStack.PushMatrix();//SceneBackground
	modelStack.Translate(center.x - 50, center.y + 20, 3);
	modelStack.Scale(5, 5, 1.f);
	RenderText(meshList[GEO_TEXT], "Background", Color(0, 0, 0));
	modelStack.PopMatrix();

	modelStack.PushMatrix();//player money
	modelStack.Translate(center.x + 30, center.y + 10, 3);
	modelStack.Scale(5, 5, 1.f);
	RenderText(meshList[GEO_TEXT], std::to_string(player->GetGold()), Color(0, 0, 0));
	modelStack.PopMatrix();
	modelStack.PushMatrix();//player money
	modelStack.Translate(center.x + 25, center.y + 20, 3);
	modelStack.Scale(5, 5, 1.f);
	RenderText(meshList[GEO_TEXT], "Money", Color(0, 0, 0));
	modelStack.PopMatrix();

	for (int i = 0; i < numButtons; ++i)//render buttons
	{
		switch (ButtArray[i]->buttype)
		{
		case Button::EDITOR_BACKGROUND_LEFT:
		{
			modelStack.PushMatrix();
			modelStack.Translate(ButtArray[i]->GetPos().x, ButtArray[i]->GetPos().y, 3);
			modelStack.Rotate(180, 0, 0, 1);
			modelStack.Scale(ButtArray[i]->GetScale().x, ButtArray[i]->GetScale().y, 1);
			RenderMesh(Buttons[ARROW], false);
			modelStack.PopMatrix();
			break;
		}
		case Button::EDITOR_BACKGROUND_RIGHT:
		{
			modelStack.PushMatrix();
			modelStack.Translate(ButtArray[i]->GetPos().x, ButtArray[i]->GetPos().y, 3);
			modelStack.Scale(ButtArray[i]->GetScale().x, ButtArray[i]->GetScale().y, 1);
			RenderMesh(Buttons[ARROW], false);
			modelStack.PopMatrix();
			break;
		}
		case Button::EDITOR_MONEY_LEFT:
		{
			modelStack.PushMatrix();
			modelStack.Translate(ButtArray[i]->GetPos().x, ButtArray[i]->GetPos().y, 3);
			modelStack.Rotate(180, 0, 0, 1);
			modelStack.Scale(ButtArray[i]->GetScale().x, ButtArray[i]->GetScale().y, 1);
			RenderMesh(Buttons[ARROW], false);
			modelStack.PopMatrix();
			break;
		}
		case Button::EDITOR_MONEY_RIGHT:
		{
			modelStack.PushMatrix();
			modelStack.Translate(ButtArray[i]->GetPos().x, ButtArray[i]->GetPos().y, 3);
			modelStack.Scale(ButtArray[i]->GetScale().x, ButtArray[i]->GetScale().y, 1);
			RenderMesh(Buttons[ARROW], false);
			modelStack.PopMatrix();
			break;
		}
		case Button::EDITOR_CANNON:
		{
			if (!iscannon)
			{
				modelStack.PushMatrix();
				modelStack.Translate(ButtArray[i]->GetPos().x, ButtArray[i]->GetPos().y, 4);
				modelStack.Scale(ButtArray[i]->GetScale().x, ButtArray[i]->GetScale().y, 1);
				RenderMesh(Buttons[LOCK], false);
				modelStack.PopMatrix();
			}
			break;
		}
		case Button::EDITOR_DRILL:
		{
			if (!isdrill)
			{
				modelStack.PushMatrix();
				modelStack.Translate(ButtArray[i]->GetPos().x, ButtArray[i]->GetPos().y, 4);
				modelStack.Scale(ButtArray[i]->GetScale().x, ButtArray[i]->GetScale().y, 1);
				RenderMesh(Buttons[LOCK], false);
				modelStack.PopMatrix();
			}
			break;
		}
		case Button::EDITOR_PICKAXE:
		{
			if (!ispickaxe)
			{
				modelStack.PushMatrix();
				modelStack.Translate(ButtArray[i]->GetPos().x, ButtArray[i]->GetPos().y, 4);
				modelStack.Scale(ButtArray[i]->GetScale().x, ButtArray[i]->GetScale().y, 1);
				RenderMesh(Buttons[LOCK], false);
				modelStack.PopMatrix();
			}
			break;
		}
		case Button::EDITOR_THUMPER:
		{
			if (!isthumper)
			{
				modelStack.PushMatrix();
				modelStack.Translate(ButtArray[i]->GetPos().x, ButtArray[i]->GetPos().y, 4);
				modelStack.Scale(ButtArray[i]->GetScale().x, ButtArray[i]->GetScale().y, 1);
				RenderMesh(Buttons[LOCK], false);
				modelStack.PopMatrix();
			}
			break;
		}
		case Button::EDITOR_MISSILE:
		{
			if (!ismissile)
			{
				modelStack.PushMatrix();
				modelStack.Translate(ButtArray[i]->GetPos().x, ButtArray[i]->GetPos().y, 4);
				modelStack.Scale(ButtArray[i]->GetScale().x, ButtArray[i]->GetScale().y, 1);
				RenderMesh(Buttons[LOCK], false);
				modelStack.PopMatrix();
			}
			break;
		}
		case Button::EDITOR_DYNAMITE:
		{
			if (!isdynamite)
			{
				modelStack.PushMatrix();
				modelStack.Translate(ButtArray[i]->GetPos().x, ButtArray[i]->GetPos().y, 4);
				modelStack.Scale(ButtArray[i]->GetScale().x, ButtArray[i]->GetScale().y, 1);
				RenderMesh(Buttons[LOCK], false);
				modelStack.PopMatrix();
			}
			break;
		}

		}
	}


}

void SceneEditor::UpdateOptions()
{
	//option buttons response and position update;
	Vector3 center(m_worldWidth * 0.5 + 2 + camera.GetOffset_x(), m_worldHeight* 0.5 + 6 + camera.GetOffset_y(), 1);
	float offset = 9.5;
	for (int i = 0; i < numButtons; ++i)
	{
		switch (ButtArray[i]->buttype)
		{
		case Button::EDITOR_BACKGROUND_LEFT:
		{
			ButtArray[i]->SetScale(Vector3(10, 10, 1));
			ButtArray[i]->SetPos(center + Vector3(-40, 10, 1));
			break;
		}
		case Button::EDITOR_BACKGROUND_RIGHT:
		{
			ButtArray[i]->SetScale(Vector3(10, 10, 1));
			ButtArray[i]->SetPos(center + Vector3(-25, 10, 1));
			break;
		}
		case Button::EDITOR_MONEY_LEFT:
		{
			ButtArray[i]->SetScale(Vector3(10, 10, 1));
			ButtArray[i]->SetPos(center + Vector3(20, 10, 1));
			break;
		}
		case Button::EDITOR_MONEY_RIGHT:
		{
			ButtArray[i]->SetScale(Vector3(10, 10, 1));
			ButtArray[i]->SetPos(center + Vector3(50, 10, 1));
			break;
		}
		case Button::EDITOR_CANNON:
		{
			ButtArray[i]->SetScale(Vector3(5, 5, 1));
			ButtArray[i]->SetPos(center + Vector3(-24 + offset, -21, 1));
			break;
		}
		case Button::EDITOR_DRILL:
		{
			ButtArray[i]->SetScale(Vector3(5, 5, 1));
			ButtArray[i]->SetPos(center + Vector3(-24 + offset * 3, -21, 1));
			break;
		}
		case Button::EDITOR_PICKAXE:
		{
			ButtArray[i]->SetScale(Vector3(5, 5, 1));
			ButtArray[i]->SetPos(center + Vector3(-24, -21, 1));//x60 y 40 10 cm size
			break;
		}
		case Button::EDITOR_THUMPER:
		{
			ButtArray[i]->SetScale(Vector3(5, 5, 1));
			ButtArray[i]->SetPos(center + Vector3(- 24 + offset * 2, -21, 1));
			break;
		}
		case Button::EDITOR_MISSILE:
		{
			ButtArray[i]->SetScale(Vector3(5, 5, 1));
			ButtArray[i]->SetPos(center + Vector3(-24 + offset * 4, -21, 1));
			break;
		}
		case Button::EDITOR_DYNAMITE:
		{
			ButtArray[i]->SetScale(Vector3(5, 5, 1));
			ButtArray[i]->SetPos(center + Vector3(-24 + offset * 5, -21, 1));
			break;
		}

		}
		
	}
}

void SceneEditor::RenderMinimap()
{
	CMinimap::GetInstance()->SetBackground(BGlist[backgroundindex]); //change bg of minimap here

															   // Push the current transformation into the modelStack
	modelStack.PushMatrix();
	modelStack.Translate(camera.GetOffset_x() + CMinimap::GetInstance()->getScale().x / 2, camera.GetOffset_y() + CMinimap::GetInstance()->getScale().y / 2, 1);
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
		RenderMesh(BGlist[backgroundindex], false); //and here
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
			if (map->Map[i][k] == 1)
			{
				modelStack.PushMatrix();

				modelStack.Scale(0.04, 0.06, 0.05);
				modelStack.Translate(((k + 1)*0.4) - 10, ((map->GetNumOfTiles_Height() - i) - 30)*0.2, 0);
				RenderMesh(BlockList[GEO_GRASS][0], false);
				modelStack.PopMatrix();

			}
			else if (map->Map[i][k] == 2)
			{
				modelStack.PushMatrix();

				modelStack.Scale(0.04, 0.06, 0.05);
				modelStack.Translate(((k + 1)*0.4) - 10, ((map->GetNumOfTiles_Height() - i) - 30)*0.2, 0);
				RenderMesh(BlockList[GEO_GLASS][0], false);
				modelStack.PopMatrix();
			}
			else if (map->Map[i][k] == 3)
			{
				modelStack.PushMatrix();

				modelStack.Scale(0.04, 0.06, 0.05);
				modelStack.Translate(((k + 1)*0.4) - 10, ((map->GetNumOfTiles_Height() - i) - 30)*0.2, 0);
				RenderMesh(BlockList[GEO_WOOD][0], false);
				modelStack.PopMatrix();
			}
			else if (map->Map[i][k] == 4)
			{
				modelStack.PushMatrix();

				modelStack.Scale(0.04, 0.06, 0.05);
				modelStack.Translate(((k + 1)*0.4) - 10, ((map->GetNumOfTiles_Height() - i) - 30)*0.2, 0);
				RenderMesh(BlockList[GEO_METAL][0], false);
				modelStack.PopMatrix();
			}
			else if (map->Map[i][k] == 5)
			{
				modelStack.PushMatrix();

				modelStack.Scale(0.04, 0.06, 0.05);
				modelStack.Translate(((k + 1)*0.4) - 10, ((map->GetNumOfTiles_Height() - i) - 30)*0.2, 0);
				RenderMesh(BlockList[GEO_BRICK][0], false);
				modelStack.PopMatrix();
			}
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

void SceneEditor::UpdateBlocks(double dt)
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
		RenderMesh(BlockList[go->Btype][0], false);
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

void SceneEditor::RenderUI(GameObject * thing)
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.GetOffset_x() + CMinimap::GetInstance()->getScale().x / 2, camera.GetOffset_y() + CMinimap::GetInstance()->getScale().y / 2, 0);
	switch (thing->Btype)
	{
	case 1:
		modelStack.PushMatrix();
		modelStack.Translate(15, 80, 1);
		modelStack.Scale(38, 11, 1);
		RenderMesh(Editorboxlist[GEO_sGLASS], false);
		modelStack.PopMatrix();

	case 2:
		modelStack.PushMatrix();
		modelStack.Translate(15, 80, 1);
		modelStack.Scale(38, 11, 1);
		RenderMesh(Editorboxlist[GEO_sWOOD], false);
		modelStack.PopMatrix();

	case 3:
		modelStack.PushMatrix();
		modelStack.Translate(15, 80, 1);
		modelStack.Scale(38, 11, 1);
		RenderMesh(Editorboxlist[GEO_sMETAL], false);
		modelStack.PopMatrix();

	case 4:
		modelStack.PushMatrix();
		modelStack.Translate(15, 80, 1);
		modelStack.Scale(38, 11, 1);
		RenderMesh(Editorboxlist[GEO_sBRICK], false);
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();
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

	RenderBG();

	RenderMinimap(); //test
	RenderUI(mapeditor->GetCurrentBlock());//render player active tool to change UI

	//RenderMesh(meshList[GEO_AXES], false);
	if (!optionsmenu)
	{
		if (mapeditor->GetIsEditing())
			RenderGO(mapeditor->GetCurrentBlock());//render  player active tool
		else
			RenderGO(player->GetActiveTool());//render  player active tool
	}

	//renderobjects
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
	//renderblocks
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

	if (optionsmenu)
	{
		RenderOptions();
	}


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

	if (mapeditor->GetIsEditing() && !optionsmenu)
	{
		ss << "Press S to save / play map";
	}
	else if (optionsmenu)
	{
		ss << "Press Enter to play map";
	}
	else if (!mapeditor->GetIsEditing() && !optionsmenu)
	{
		ss << "Press Enter to edit map";
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 0);

	ss.str(std::string());
	ss.precision(3);
	ss << "Blocks: " << m_objectCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	ss.str(std::string());
	ss.precision(5);
	ss << "FPS: " << fps;
	// ss << "minimappos: " << CMinimap::GetInstance()->getPosition();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 3, 0, 57);
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
