#include "SceneAsteroid.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAsteroid::SceneAsteroid()
{
}

SceneAsteroid::~SceneAsteroid()
{
}

void SceneAsteroid::Init()
{
	SceneBase::Init();
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;
	
	Math::InitRNG();

	//Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	for (size_t i = 0; i < 100; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_ASTEROID);
		m_goList.push_back(go);
	}
	/*for (size_t i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_ENEMY);
		m_enemyShip.push_back(go);
	}*/

	//Exercise 2b: Initialize m_lives and m_score
	m_lives = 3;
	m_score = 0;

	//Exercise 2c: Construct m_ship, set active, type, scale and pos
	m_ship = new GameObject(GameObject::GO_SHIP);
	m_ship->active = true;
	m_ship->scale = Vector3(5, 5, 5);
	m_ship->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 0);
	m_ship->momentOfInertia = 1.0f;
	m_ship->angularVelocity = 0;
	m_ship->dir.Set(1, 0, 0);
}

GameObject* SceneAsteroid::FetchGO()
{
	//Exercise 3a: Fetch a game object from m_goList and return it
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}
	for (size_t i = 0; i < 10; i++)
	{
		GameObject *go = new GameObject(GameObject::GO_ASTEROID);
		m_goList.push_back(go);
		m_worldHeight;
	}
	return m_goList[m_goList.size() - 1];
}

void SceneAsteroid::Update(double dt)
{
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	SceneBase::Update(dt);
	
	if(Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}
	m_force.SetZero();
	m_torque.SetZero();
	//Exercise 6: set m_force values based on WASD
	if(Application::IsKeyPressed('W'))
	{
		m_force += m_ship->dir * 100;
	}
	if(Application::IsKeyPressed('A'))
	{
		m_force += m_ship->dir * 5;
		Vector3 r(1, -1, 0);
		Vector3 F(0, 5, 0);
		m_torque += r.Cross(F);
	}
	if(Application::IsKeyPressed('S'))
	{
		m_force -= m_ship->dir * 100;
	}
	if(Application::IsKeyPressed('D'))
	{
		m_force += m_ship->dir * 5;
		Vector3 r(-1, -1, 0);
		Vector3 F(0, 5, 0);
		m_torque += r.Cross(F);
	}
	//Exercise 8: use 2 keys to increase and decrease mass of ship
	if (Application::IsKeyPressed('N') && m_ship->mass > 0.1)
	{
		m_ship->mass -= 0.1;
	}
	if (Application::IsKeyPressed('M'))
	{
		m_ship->mass += 0.1;
	}

	//Exercise 11: use a key to spawn some asteroids
	static bool bVState = false;
	if (Application::IsKeyPressed('V') && !bVState)
	{
		bVState = true;
	}
	else if (!Application::IsKeyPressed('V') && bVState)
	{
		bVState = false;

		for (int i = 0; i < 25; ++i)
		{
			GameObject *go = FetchGO();
			go->active = true;
			go->type = GameObject::GO_ASTEROID;
			go->scale.Set(1, 1, 1);
			go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 0);
			go->vel.Set(Math::RandFloatMinMax(-5.0f, 5.0f), Math::RandFloatMinMax(-5.0f, 5.0f), 0);
		}
	}

	//Exercise 14: use a key to spawn a bullet
	static bool bSpaceState = false;
	if (Application::IsKeyPressed(VK_SPACE) && !bSpaceState)
	{
		bSpaceState = true;
	}
	else if (!Application::IsKeyPressed(VK_SPACE) && bSpaceState)
	{
		bSpaceState = false;
		GameObject *go = FetchGO();
		go->active = true;
		go->type = GameObject::GO_BULLET;
		go->scale.Set(1, 1, 1);
		go->pos.Set(m_ship->pos.x, m_ship->pos.y, 0);
		if (m_ship->vel.IsZero())
			go->vel.Set(BULLET_SPEED, 0, 0);
		else
			go->vel.Set(m_ship->dir.Normalized().x * BULLET_SPEED, m_ship->dir.Normalized().y * BULLET_SPEED, 0);
	}

	//static bool bZState = false;
	//if (Application::IsKeyPressed('Z') && !bZState)
	//{
	//	bZState = true;
	//}
	//else if (!Application::IsKeyPressed('Z') && bZState)
	//{
	//	bZState = false;
	//	GameObject *go = FetchGO();
	//	go->active = true;
	//	go->type = GameObject::GO_ENEMY;
	//	go->scale.Set(1, 1, 1);
	//	go->pos.Set(m_ship->pos.x, m_ship->pos.y, 0);
	//	if (m_ship->vel.IsZero())
	//		go->vel.Set(BULLET_SPEED, 0, 0);
	//	else
	//		go->vel.Set(m_ship->dir.Normalized().x * BULLET_SPEED, m_ship->dir.Normalized().y * BULLET_SPEED, 0);
	//}

	//Mouse Section
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	//Physics Simulation Section

	//Exercise 7: Update ship's velocity based on m_force
	if (!m_force.IsZero())
	{
		Vector3 da = m_force * (1.f / m_ship->mass);
		m_ship->vel += da * dt * m_speed;
		if (m_ship->vel.Length() > MAX_SPEED)
		{
			m_ship->vel.Normalize();
			m_ship->vel *= MAX_SPEED;
		}
	}
	m_ship->pos += m_ship->vel * dt * m_speed;

	if (!m_torque.IsZero())
	{
		m_ship->momentOfInertia = m_ship->mass * 1 * 1;
		float angularAcceleration = m_torque.z * (1 / m_ship->momentOfInertia);
		m_ship->angularVelocity += angularAcceleration * dt * m_speed;
		if (m_ship->angularVelocity > MAX_SPEED)
		{
			m_ship->angularVelocity = MAX_SPEED;
		}
	}
	float theta = atan2(m_ship->dir.y, m_ship->dir.x);
	theta += m_ship->angularVelocity * dt * m_speed;
	m_ship->dir.Set(cos(theta), sin(theta), 0);

	if(!m_ship->dir.IsZero())
		m_ship->dir.Normalize();

	//Exercise 9: wrap ship position if it leaves screen
	if (m_ship->pos.x > m_worldWidth + 2)
		m_ship->pos.x -= m_worldWidth + 2;
	else if (m_ship->pos.x < -2)
		m_ship->pos.x += m_worldWidth + 2;
	if (m_ship->pos.y > m_worldHeight + 2)
		m_ship->pos.y -= m_worldHeight + 2;
	else if (m_ship->pos.y < -2)
		m_ship->pos.y += m_worldHeight + 2;
	

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			go->pos = go->pos + (go->vel * dt * m_speed);
			if (go->type == GameObject::GO_ASTEROID)
			{
				//Exercise 12: handle collision between GO_SHIP and GO_ASTEROID using simple distance-based check
				float lengthSQ = (m_ship->pos - go->pos).LengthSquared();
				float radiusSQ = (go->scale.x + m_ship->scale.x) * (go->scale.x + m_ship->scale.x);
				if (lengthSQ < radiusSQ - 25)
				{
					m_ship->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 0);
					m_ship->vel.SetZero();
					m_lives--;
					go->active = false;
					if (m_lives == 0)
					{
						m_lives = 3;
						m_score = 0;
						for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
						{
							GameObject *go2 = (GameObject *)*it2;
							go2->active = false;
						}
						break;
					}
				}
				//Exercise 13: asteroids should wrap around the screen like the ship

				if (go->pos.x > m_worldWidth + 2)
					go->pos.x = 0;
				else if (go->pos.x < -2)
					go->pos.x = m_worldWidth;
				if (go->pos.y > m_worldHeight + 2)
					go->pos.y = 0;
				else if (go->pos.y < -2)
					go->pos.y = m_worldHeight;
			}

			if (go->type == GameObject::GO_BULLET)
			{
				//Exercise 16: unspawn bullets when they leave screen
				if (go->type == GameObject::GO_BULLET)
				{
					if (go->pos.x > m_worldWidth + 1.0f
						|| go->pos.x < -1.0f
						|| go->pos.y > m_worldHeight + 1.0f
						|| go->pos.y < -1.0f)
						go->active = false;
				}
				//Exercise 18: collision check between GO_BULLET and GO_ASTEROID
				for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					GameObject *go2 = (GameObject *)*it2;
					if (go2->active && go2->type == GameObject::GO_ASTEROID)
					{
						if ((go->pos - go2->pos).Length() < 2)
						{
							go->active = false;
							go2->active = false;
							m_score += 2;
							break;
						}
					}
				}
			}
		}
	}
}


void SceneAsteroid::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_SHIP:
		//Exercise 4a: render a sphere with radius 1
		/*modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();*/

		//Exercise 17a: render a ship texture or 3D ship model
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0);
		modelStack.Scale(m_ship->scale.x, m_ship->scale.y, m_ship->scale.z);
		//Exercise 17b:	re-orientate the ship with velocity
		modelStack.Rotate(Math::RadianToDegree(atan2(m_ship->dir.y, m_ship->dir.x)) - 90, 0, 0, 1);
		RenderMesh(meshList[GEO_SHIP], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ASTEROID:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BULLET:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0);
		modelStack.Scale(0.2, 0.2, 0.2);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneAsteroid::Render()
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

	if (m_ship->active)
		RenderGO(m_ship);

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}

	//On screen text

	//Exercise 5a: Render m_lives, m_score
	std::ostringstream ss3;
	ss3.str("");
	ss3 << "Lives: " << m_lives;
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 1, 9);
	ss3.str("");
	ss3 << "Score: " << m_score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 30, 9);
	//Exercise 5b: Render position, velocity & mass of ship
	ss3.str("");
	ss3 << "Ship's Pos: " << "[" << (int)m_ship->pos.x << ", " << (int)m_ship->pos.y << "]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 1, 12);
	ss3.str("");
	ss3 << "Ship's Vel: " << "[" << (int)m_ship->vel.x << ", " << (int)m_ship->vel.y << "]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 1, 15);
	ss3.str("");
	ss3 << "Ship's Mass: " << m_ship->mass;
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 30, 6);

	std::ostringstream ss2;
	ss2.precision(3);
	ss2 << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 3, 1, 6);
	
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 1, 3);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Asteroid", Color(0, 1, 0), 3, 1, 0);
}

void SceneAsteroid::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ship)
	{
		delete m_ship;
		m_ship = NULL;
	}
}
