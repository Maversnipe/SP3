#include "SceneGameOver.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "SceneManager.h"

SceneGameOver::SceneGameOver()
{
	SceneBase::Init();
}

SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	float textscale = 1.5;
}

void SceneGameOver::Update(double dt)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float posX = static_cast<float>(x) / w * m_worldWidth + camera.GetOffset_x();
	float posY = (h - static_cast<float>(y)) / h * m_worldHeight + camera.GetOffset_y();
	Vector3 mousepos(posX, posY, 0);

	if (Application::IsKeyPressed(VK_RETURN))
		SceneManager::currscene = 1;
}


void SceneGameOver::Render()
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

	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float posX = static_cast<float>(x) / w * m_worldWidth + camera.GetOffset_x();
	float posY = (h - static_cast<float>(y)) / h * m_worldHeight + camera.GetOffset_y();
	Vector3 mousepos(posX, posY, 0);

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2 + 2, m_worldHeight / 2 + 6, 0);
	modelStack.Scale(m_worldWidth, m_worldHeight, 1);
	RenderMesh(Gameover[END_MENU], false);
	modelStack.PopMatrix();
}

void SceneGameOver::Exit()
{
}
