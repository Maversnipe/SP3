#include "SceneMainMenu.h"
#include "GL\glew.h"
#include "Application.h"
#include "SceneManager.h"
#include <sstream>


SceneMainMenu::SceneMainMenu()
{
	SceneBase::Init();
}

SceneMainMenu::~SceneMainMenu()
{
}

void SceneMainMenu::Init()
{
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	float textscale = 1.5;

	ButtArray[0] = new Button(Vector3(22.6 * textscale, 4.9 * textscale), Vector3(m_worldWidth / 2 + 52, m_worldHeight / 2 + 36, 0),Button::BUTTON_TYPE::BUTTON_PLAY);
	ButtArray[1] = new Button(Vector3(32.6 * textscale, 4.9 * textscale), ButtArray[0]->GetPos() - Vector3(0, ButtArray[0]->GetScale().y*2),Button::BUTTON_TYPE::BUTTON_SANDBOX);
	ButtArray[2] = new Button(Vector3(27.6 * textscale, 4.9 * textscale), ButtArray[1]->GetPos() - Vector3(0, ButtArray[1]->GetScale().y * 2), Button::BUTTON_TYPE::BUTTON_OPTIONS);
	ButtArray[3] = new Button(Vector3(17.8 * textscale, 5.6 * textscale), ButtArray[2]->GetPos() - Vector3(0, ButtArray[2]->GetScale().y * 2), Button::BUTTON_TYPE::BUTTON_EXIT);
}

void SceneMainMenu::Update(double dt)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float posX = static_cast<float>(x) / w * m_worldWidth + camera.GetOffset_x();
	float posY = (h - static_cast<float>(y)) / h * m_worldHeight + camera.GetOffset_y();
	Vector3 mousepos(posX, posY, 0);
	//Mouse Section
	  static bool bLButtonState = false;
	  if(!bLButtonState && Application::IsMousePressed(0))
	  {
	      bLButtonState = true;
	      std::cout << "LBUTTON DOWN" << std::endl;
		  for (unsigned i = 0; i < numButtons; ++i)
		  {
			  if (ButtArray[i]->MouseCheck(mousepos))
			  {
				  switch(ButtArray[i]->buttype)
				  {
				  case(Button::BUTTON_PLAY):
					  SceneManager::currscene = 2;
					  break;
				  case(Button::BUTTON_SANDBOX):
					  SceneManager::currscene = 3;
					  break;
				  case(Button::BUTTON_OPTIONS):
					  break;
				  case(Button::BUTTON_EXIT):
					  break;
				  }

			  }
		  }
	  }
	  else if(bLButtonState && !Application::IsMousePressed(0))
	  {
	      bLButtonState = false;
	      std::cout << "LBUTTON UP" << std::endl;
	  }
	  for (unsigned i = 0; i < numButtons; ++i)
	  {
		  if (ButtArray[i]->MouseCheck(mousepos) && !ButtArray[i]->GetBig())
		  {
			  ButtArray[i]->SetScale(ButtArray[i]->GetScale()*1.1);
			  ButtArray[i]->SetBig(true);
		  }
		  else if(!ButtArray[i]->MouseCheck(mousepos) && ButtArray[i]->GetBig())
		  {
			  ButtArray[i]->SetScale(ButtArray[i]->GetScale()*0.91);
			  ButtArray[i]->SetBig(false);
		  }
	  }

	  CSoundEngine::GetInstance()->PlayASound("test"); //test sound
}

void SceneMainMenu::Render()
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

	//RenderTest(mousepos);
	//RenderText();
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth/2 + 2, m_worldHeight/2+6, 0);
	modelStack.Scale(m_worldWidth, m_worldHeight, 1);
	RenderMesh(MenuButtons[MENU_BACKGROUND], false);
	modelStack.PopMatrix();


	for (unsigned i = 1; i <= numButtons; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(ButtArray[i - 1]->GetPos().x, ButtArray[i - 1]->GetPos().y, 1);
		modelStack.Scale(ButtArray[i-1]->GetScale().x, ButtArray[i - 1]->GetScale().y, 1);
		RenderMesh(MenuButtons[i], false);
		modelStack.PopMatrix();
	}

}

void SceneMainMenu::Exit()
{
}

void SceneMainMenu::RenderText()
{
	//text, screen is 80x by 60y

}

void SceneMainMenu::RenderTest(Vector3 mousepos)
{
	modelStack.PushMatrix();
	modelStack.Translate(mousepos.x, mousepos.y, mousepos.z);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_BALL], false);
	modelStack.PopMatrix();
}
