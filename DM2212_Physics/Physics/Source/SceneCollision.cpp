#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision()
{
}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
{
    SceneBase::Init();

    //Map reading
    map = new FileIO();
    map->Init(Application::GetWindowHeight() * 4.f, Application::GetWindowWidth() * 4.f, 40, 64, Application::GetWindowHeight() * 2.f, Application::GetWindowWidth() * 2.f, 30, 30);
    map->Read("Maps//test.csv");
    RenderMap();

    //Player
    player = PlayerInfo::GetInstance();
    player->Init();

    //Physics code here
    m_speed = 1.f;
    
    Math::InitRNG();

    m_objectCount = 0;

    m_ghost = new GameObject(GameObject::GO_WALL);
	m_Block = new Block();

    initialKE = 0.0f;
    finalKE = 0.0f;

	m_vec3Gravity.Set(0, -9.8, 0);
	
	CollisionManager::getCManager()->SetWorldSize(Application::GetWindowHeight(), Application::GetWindowWidth());
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

	Block *go = new Block(0, 1, true);
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

	Vector3 mousepos(posX, posY, 0);

    SceneBase::Update(dt);
    player->Update(dt, mousepos);//updates player and tools
    
    if(Application::IsKeyPressed('9'))
    {
        m_speed = Math::Max(0.f, m_speed - 0.1f);
    }
    if(Application::IsKeyPressed('0'))
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

    //Mouse Section
    //static bool bLButtonState = false;
    //if(!bLButtonState && Application::IsMousePressed(0))
    //{
    //    bLButtonState = true;
    //    std::cout << "LBUTTON DOWN" << std::endl;
    //    
    //    double x, y;
    //    Application::GetCursorPos(&x, &y);
    //    int w = Application::GetWindowWidth();
    //    int h = Application::GetWindowHeight();
    //    float posX = static_cast<float>(x) / w * m_worldWidth + camera.GetOffset_x();
    //    float posY = (h - static_cast<float>(y)) / h * m_worldHeight + camera.GetOffset_y();

    //    m_ghost->pos.Set(posX, posY, 0); //IMPT
    //    float sc = 2;
    //    m_ghost->scale.Set(sc, sc, sc);
    //}
    //else if(bLButtonState && !Application::IsMousePressed(0))
    //{
    //    bLButtonState = false;
    //    std::cout << "LBUTTON UP" << std::endl;

    //    //spawn small GO_BALL
    //    GameObject *go = FetchGO();
    //    go->type = GameObject::GO_BALL;
    //    double x, y;
    //    Application::GetCursorPos(&x, &y);
    //    int w = Application::GetWindowWidth();
    //    int h = Application::GetWindowHeight();
    //    float posX = static_cast<float>(x) / w * m_worldWidth + camera.GetOffset_x();
    //    float posY = (h - static_cast<float>(y)) / h * m_worldHeight + camera.GetOffset_y();

    //    go->pos = m_ghost->pos;
    //    go->vel.Set(m_ghost->pos.x - posX, m_ghost->pos.y - posY, 0);
    //    m_ghost->active = false;
    //    float sc = go->vel.Length();
    //    sc = Math::Clamp(sc, 2.f, 10.f);
    //    go->scale.Set(sc, sc, sc);
    //    go->mass = (sc * sc * sc);
    //}
    static bool bRButtonState = false;
    if(!bRButtonState && Application::IsMousePressed(1))
    {
        bRButtonState = true;
        std::cout << "RBUTTON DOWN" << std::endl;

        m_ghost->pos.Set(posX, posY, 0); //IMPT
        m_ghost->active = true;
        float sc = 3;
        m_ghost->scale.Set(sc, sc, sc);
    }
    else if(bRButtonState && !Application::IsMousePressed(1))
    {
        bRButtonState = false;
        std::cout << "RBUTTON UP" << std::endl;

        GameObject *go = FetchGO();
        go->type = GameObject::GO_BALL;
		go->tooltype = GameObject::TOOL_TYPE::CANNONBALL;
        go->pos = m_ghost->pos;
        go->vel.Set(m_ghost->pos.x - posX, m_ghost->pos.y - posY, 0);
        m_ghost->active = false;
        float sc = 3.f;
        go->scale.Set(sc, sc, sc);
        go->mass = 3.f;
		go->aabb.SetAABB(go->pos, go->scale);

       /* m_timeEstimated1 = 10000.f;
        m_timeTaken1 = 0.f;
        m_timerStarted = true;

        for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
        {
            GameObject *go2 = (GameObject *)*it;
            if (go2->active && go != go2)
            {
                float time = CheckCollision2(go, go2);
                if (time > 0.f && time < m_timeEstimated1)
                {
                    m_timeEstimated1 = time;
                }
            }
        }*/
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
                go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
                go->Btype = GameObject::BLOCK_TYPE::GO_GRASS;
				go->aabb.SetAABB(go->pos, go->scale);
				
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
            }
			else if (map->Map[i][k] == 5)
			{
				Block *go = FetchGo1();
				go->type = GameObject::GO_BLOCK;
				go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(-5, 9.8, 0);
				go->mass = 1.f;
				go->Btype = GameObject::BLOCK_TYPE::GO_BRICK;
			}
            else if (map->Map[i][k] == 10)
            {
                GameObject *go = FetchGO();
                go->type = GameObject::GO_WALL;
                go->pos = Vector3((k + 1) * 4, (map->GetNumOfTiles_Height() - i) * 4, 0);
				go->scale.Set(4.f, 4.f, 1.f);
				go->vel.Set(0, 0, 0);
				go->mass = 1.f;
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

void SceneCollision::UpdateObjects(double dt)
{
	for (auto &i : m_goList)
	{
		//i->Update(dt);
		if (i->tooltype == GameObject::TOOL_TYPE::CANNONBALL)
		{
			Cannonball* cannonball = static_cast<Cannonball*>(i);
			cannonball->Update(m_goList, m_vBlocks, dt);
		}

	}
}

void SceneCollision::UpdateBlocks(double dt)
{
	for (auto &i : m_vBlocks)
	{
		//i->Update(m_goList, m_vBlocks, dt);

		if (i->Btype == GameObject::BLOCK_TYPE::GO_GRASS)
		{
			Grassblock* b = static_cast<Grassblock*>(i);

			if (b != NULL)
			{
				b->Update(m_goList, m_vBlocks, dt);
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_GLASS)
		{
			Glassblock* b = static_cast<Glassblock*>(i);

			if (b != NULL)
			{
				b->Update(m_goList, m_vBlocks, dt);
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_WOOD)
		{
			Woodblock* b = static_cast<Woodblock*>(i);

			if (b != NULL)
			{
				b->Update(m_goList, m_vBlocks, dt);
			}
		}
		else if (i->Btype == GameObject::BLOCK_TYPE::GO_METAL)
		{
			Metalblock* b = static_cast<Metalblock*>(i);

			if (b != NULL)
			{
				b->Update(m_goList, m_vBlocks, dt);
			}
		}
		else if(i->Btype == GameObject::BLOCK_TYPE::GO_BRICK)
		{
			Brickblock* b = static_cast<Brickblock*>(i);

			if (b != NULL)
			{
				b->Update(m_goList, m_vBlocks, dt);
			}
		}
	}
}

float SceneCollision::CheckCollision2(GameObject *go, GameObject *go2)
{
    Vector3 relVel = go->vel - go2->vel;
    Vector3 dir = go->pos - go2->pos;
    float r = go->scale.x + go2->scale.x;
    if (relVel.Dot(dir) > 0.f)
        return -1.f;
    float a = relVel.Dot(relVel);
    float b = 2.f * relVel.Dot(dir);
    float c = dir.Dot(dir) - r * r;
    float discriminant = b * b - 4.f * a * c;
    
    if (discriminant < 0.f)
        return -1.f;
    
    float t = (-b - sqrt(discriminant)) / (2.f * a);
    
    if(t < 0.f)
        t = (-b + sqrt(discriminant)) / (2.f * a);

    return t;
}

void SceneCollision::RenderGO(GameObject *go)
{
    switch(go->type)
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
        //modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)) + go->rotation, 0.f, 0.f, 1.f);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)), 0.f, 0.f, 1.f);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        RenderMesh(BlockList[go->Btype], false);
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
    
    RenderMesh(meshList[GEO_AXES], false);

    for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject *go = (GameObject *)*it;
        if(go->active)
        {
			if(go->pos.x > camera.position.x && go->pos.x < camera.position.x + Application::GetWindowWidth())
				RenderGO(go);
        }
    }
	for (std::vector<Block *>::iterator it = m_vBlocks.begin(); it != m_vBlocks.end(); ++it)
	{
		Block *go = (Block *)*it;
		if (go->active)
		{
			if (go->pos.x > camera.position.x && go->pos.x < camera.position.x + Application::GetWindowWidth())
				RenderGO(go);
		}
	}

    if(m_ghost->active)
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
    
    ss.str(std::string());
    ss.precision(5);
    ss << "FPS: " << fps;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
    
    RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
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
}
