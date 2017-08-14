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

	//Physics code here
	m_speed = 1.f;
	
	Math::InitRNG();

	m_objectCount = 0;

	m_ghost = new GameObject(GameObject::GO_BALL);

	initialKE = 0.0f;
	finalKE = 0.0f;

	Vector3 centre(50.f, 50.f, 0.f);
	for (int i = 0; i < 8; i++)
	{
		GameObject *go = FetchGO();
		go->type = GameObject::GO_WALL;
		go->dir.Set(cos(Math::DegreeToRadian(i * 45.f)), sin(Math::DegreeToRadian(i * 45.f)), 0.f);
		go->pos = centre + go->dir * 40.f;
		go->scale.Set(4.f, 40.f, 1.f);
	}
	GameObject *go = FetchGO();
	go->type = GameObject::GO_WALL;
	go->dir.Set(0.f, 1.f, 0.f);
	go->pos = centre;
	go->scale.Set(4.f, 40.f, 1.f);
	
	go = FetchGO();
	go->type = GameObject::GO_PILLAR;
	go->pos = centre + Vector3(20.f, 0.f, 0.f);
	go->scale.Set(2.f, 2.f, 1.f);
	go = FetchGO();
	go->type = GameObject::GO_PILLAR;
	go->pos = centre + Vector3(-20.f, 0.f, 0.f);
	go->scale.Set(2.f, 2.f, 1.f);

	//Sprite animation ^-^
	go = FetchGO();
	go->type = GameObject::GO_TEST_ANIMATION;
	go->scale.Set(1.f, 1.f, 1.f);
	go->pos = centre;

	//Test Block spawn
	go = FetchGO();
	go->type = GameObject::GO_BLOCK;
	go->pos = centre + Vector3(-20.f, 20.f, 0.f);
	go->scale.Set(2.f, 2.f, 1.f);
	go->Btype = GameObject::BLOCK_TYPE::GO_GRASS;
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
	for(unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(go);
	}
	GameObject *go = m_goList.back();
	go->active = true;
	++m_objectCount;
	return go;
}

void SceneCollision::Update(double dt)
{
	SceneBase::Update(dt);
	
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

		//spawn small GO_BALL
		GameObject *go = FetchGO();
		go->type = GameObject::GO_BALL;
		go->pos.Set(m_worldWidth * 0.9f, 10.f);
		go->vel.Set(0.f, 10.f, 0.f);
		go->scale.Set(5.f, 5.f, 5.f);
		go->mass = (go->scale.x * go->scale.y * go->scale.z) / 8.f;
	}

	//Mouse Section
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;

		m_ghost->pos.Set(posX, posY, 0); //IMPT
		float sc = 2;
		m_ghost->scale.Set(sc, sc, sc);
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		//spawn small GO_BALL
		GameObject *go = FetchGO();
		go->type = GameObject::GO_BALL;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;

		go->pos = m_ghost->pos;
		go->vel.Set(m_ghost->pos.x - posX, m_ghost->pos.y - posY, 0);
		m_ghost->active = false;
		float sc = go->vel.Length();
		sc = Math::Clamp(sc, 2.f, 10.f);
		go->scale.Set(sc, sc, sc);
		go->mass = (sc * sc * sc) / 8.f;
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;

		m_ghost->pos.Set(posX, posY, 0); //IMPT
		m_ghost->active = true;
		float sc = 3;
		m_ghost->scale.Set(sc, sc, sc);
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		//spawn large GO_BALL
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;

		GameObject *go = FetchGO();
		go->type = GameObject::GO_BALL;
		go->pos = m_ghost->pos;
		go->vel.Set(m_ghost->pos.x - posX, m_ghost->pos.y - posY, 0);
		m_ghost->active = false;
		float sc = 3.f;
		go->scale.Set(sc, sc, sc);
		go->mass = (sc * sc * sc) / 8.f;

		m_timeEstimated1 = 10000.f;
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
		}
	}

	//Physics Simulation Section
	if (m_timerStarted)
		m_timeTaken1 += dt;

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active && go->type == GameObject::GO_BALL)
		{
			go->pos += go->vel * static_cast<float>(dt);

			//Exercise 2a: Rebound game object at screen edges
			// Rebound on X
			if (go->pos.x < 0 + go->scale.x && go->vel.x < 0)
				go->vel.x = -go->vel.x;
			else if (go->pos.x > m_worldWidth - go->scale.x && go->vel.x > 0)
				go->vel.x = -go->vel.x;
			// Rebound on Y
			if (go->pos.y < 0 + go->scale.y && go->vel.y < 0)
				go->vel.y = -go->vel.y;
			else if (go->pos.y > m_worldHeight - go->scale.y && go->vel.y > 0)
				go->vel.y = -go->vel.y;
			//Exercise 2b: Unspawn if it really leave the screen
			if (go->pos.x > m_worldWidth || go->pos.x < 0 || go->pos.y > m_worldHeight || go->pos.y < 0)
			{
				go->active = false;
				--m_objectCount;
				continue;
			}
		}

		for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != it; ++it2)
		{
			GameObject *go2 = (GameObject *)(*it2);
			if (!go2->active)
				continue;
			if (go->type != GameObject::GO_BALL && go2->type != GameObject::GO_BALL)
				continue;

			GameObject *goA, *goB;

			if (go->type == GameObject::GO_BALL)
			{
				goA = go;
				goB = go2;
			}
			else
			{
				goA = go2;
				goB = go;
			}


			if (CheckCollision(goA, goB, dt))
			{
				m_timerStarted = false;

				m1 = goA->mass;
				m2 = goB->mass;
				u1 = goA->vel;
				u2 = goB->vel;

				CollisionResponse(goA, goB);

				v1 = goA->vel;
				v2 = goB->vel;

				break;
				//Exercise 3: audit kinetic energy
			}
		}
	}
}

void SceneCollision::CollisionResponse(GameObject *go, GameObject *go2)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		Vector3 u1 = go->vel;
		Vector3 u2 = go2->vel;
		Vector3 N = (go2->pos - go->pos).Normalize();
		Vector3 u1N = u1.Dot(N) * N;
		Vector3 u2N = u2.Dot(N) * N;
		go->vel = u1 + 2.f * m2 / (m1 + m2) * (u2N - u1N);
		go2->vel = u2 + 2.f * m1 / (m1 + m2) * (u1N - u2N);

		break;
	}
	case GameObject::GO_WALL:
	{
		Vector3 vel = go->vel;
		Vector3 N = go2->dir;
		go->vel = vel - (2.f * vel.Dot(N)) * N;
		break;
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 vel = go->vel;
		Vector3 N = (go2->pos - go->pos).Normalize();
		go->vel = vel - (2.f * vel.Dot(N)) * N;
		break;
	}
	}	
}

bool SceneCollision::CheckCollision(GameObject *go, GameObject *go2, float dt)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		Vector3 p1 = go->pos;
		Vector3 p2 = go2->pos;
		Vector3 relDisplacement = p1 - p2;
		Vector3 u1 = go->vel;
		Vector3 u2 = go2->vel;
		Vector3 relVel = u1 - u2;
		float r1 = go->scale.x;
		float r2 = go2->scale.x;

		return relDisplacement.LengthSquared() < (r1 + r2) * (r1 * r2) &&
			relVel.Dot(relDisplacement) < 0.f;
	}
	case GameObject::GO_WALL:
	{
		Vector3 w0 = go2->pos;
		Vector3 b1 = go->pos;
		Vector3 N = go2->dir;
		Vector3 NP = N.Cross(Vector3(0.f, 0.f, 1.f));
		float r = go->scale.x;
		float h = go2->scale.x;
		float l = go2->scale.y;
		
		if ((w0 - b1).Dot(N) < 0.f)
			N = -N;

		return (go->vel.Dot(N) > 0.f) &&
			((abs((w0 - b1).Dot(N)) < r + h * 0.5f) &&
			(abs((w0 - b1).Dot(NP)) < r + l * 0.5f));
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 p1 = go->pos;
		Vector3 p2 = go2->pos;
		float r1 = go->scale.x;
		float r2 = go2->scale.x;

		return ((p2 - p1).LengthSquared() < ((r1 + r2) * (r1 * r2))) &&
			((p2 - p1).LengthSquared() > 0.f) &&
			((p2 - p1).Dot(go->vel) > 0.f);
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
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
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
			RenderGO(go);
		}
	}
	if(m_ghost->active)
		RenderGO(m_ghost);

	//On screen text
	std::ostringstream ss;
	ss << "Time Estimated: " << m_timeEstimated1;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 18);

	ss.str(std::string());
	ss.precision(5);
	ss << "Time Taken: " << m_timeTaken1;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);

	ss.str(std::string());
	ss.precision(5);
	ss << "E1: " << initialKE;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);
	
	ss.str(std::string());
	ss.precision(5);
	ss << "E2: " << finalKE;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

	//Exercise 3: render initial and final kinetic energy
	
	ss.str(std::string());
	ss.precision(3);
	ss << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);
	
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
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
