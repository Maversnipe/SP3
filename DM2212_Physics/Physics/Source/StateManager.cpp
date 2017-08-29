#include "StateManager.h"
#include "Application.h"
#include "SceneManager.h"
#include <sstream>
#include <string>

StateManager* StateManager::statemanager;

StateManager::~StateManager()
{
}

StateManager * StateManager::getInstance()
{
	if (!statemanager)
	{
		statemanager = new StateManager;
	}
	return statemanager;
}

void StateManager::Init()
{
	//Initialise gamestate
	this->m_currstate = S_PLAYING;

	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	//glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//comg.vertexshader", "Shader//comg.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	camera.Init(Vector3(2.f, 6.f, 5), Vector3(2.f, 6.f, 0), Vector3(0, 1, 0));

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	//PAUSE
	MenuButton[PAUSE_MENU] = MeshBuilder::GenerateQuad("backround", Color(1.f, 0.f, 0.f), 1.f);
	MenuButton[PAUSE_MENU]->textureID = LoadTGA("Image//textbox.tga");
	MenuButton[PAUSE_CONTINUE] = MeshBuilder::GenerateQuad("play", Color(1.f, 0.f, 0.f), 1.f);
	MenuButton[PAUSE_CONTINUE]->textureID = LoadTGA("Image//mainmenu//continue.tga");
	MenuButton[PAUSE_OPTIONS] = MeshBuilder::GenerateQuad("options", Color(1.f, 0.f, 0.f), 1.f);
	MenuButton[PAUSE_OPTIONS]->textureID = LoadTGA("Image//mainmenu//options.tga");
	MenuButton[PAUSE_EXIT] = MeshBuilder::GenerateQuad("exit", Color(1.f, 0.f, 0.f), 1.f);
	MenuButton[PAUSE_EXIT]->textureID = LoadTGA("Image//mainmenu//quit.tga");

	//Meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_BALL] = MeshBuilder::GenerateQuad("ball", Color(1, 1, 1), 2.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 1.f);
	meshList[GEO_SHIP] = MeshBuilder::GenerateQuad("ship", Color(1, 1, 1), 1.f);
	meshList[GEO_SHIP]->textureID = LoadTGA("Image//rocket.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_GRID] = MeshBuilder::GenerateQuad("grid", Color(1, 1, 1), 1.f);
	meshList[GEO_GRID]->textureID = LoadTGA("Image//Block_White.tga");
	meshList[GEO_TEST_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("phish", 1, 6, 1.f);
	meshList[GEO_TEST_ANIMATION]->textureID = LoadTGA("Image//maskfish.tga");
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_TEST_ANIMATION]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 5, 0, 1.f, true);
	}
	meshList[GEO_TEXTBOX] = MeshBuilder::GenerateQuad("ship", Color(1, 1, 1), 1.f);
	meshList[GEO_TEXTBOX]->textureID = LoadTGA("Image//textbox.tga");
	meshList[GEO_ITEMSELECT] = MeshBuilder::GenerateQuad("items", Color(1, 1, 1), 1.f);
	meshList[GEO_ITEMSELECT]->textureID = LoadTGA("Image//toolboxbase2.tga");

	//Set limits
	m_fworldheight = 100.f;
	m_fworldwidth = m_fworldheight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();

	float textscale = 1.5;

	Array[0] = new Button(Vector3(25.6 * textscale, 5.6 * textscale), Vector3(132, 78.5, 1.f), Button::BUTTON_TYPE::BUTTON_PLAY);
	Array[1] = new Button(Vector3(27.6 * textscale, 4.9 * textscale), Vector3(132, 58.5, 1.f), Button::BUTTON_TYPE::BUTTON_OPTIONS);
	Array[2] = new Button(Vector3(25.6 * textscale, 5.6 * textscale), Vector3(132, 36.5, 1.f), Button::BUTTON_TYPE::BUTTON_EXIT);

	bLightEnabled = false;
}

void StateManager::Update(double dt)
{
	if (Application::IsKeyPressed(VK_ESCAPE))
	{
		StateManager::getInstance()->ChangeState(S_PAUSED);
	}

	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Mouse
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float posX = static_cast<float>(x) / w * m_fworldwidth + camera.GetOffset_x();
	float posY = (h - static_cast<float>(y)) / h * m_fworldheight + camera.GetOffset_y();
	Vector3 mousepos(posX, posY, 0);

	if (m_currstate == S_PAUSED)
	{
		//Mouse Section
		static bool bLButtonState = false;
		if (!bLButtonState && Application::IsMousePressed(0))
		{
			bLButtonState = true;
			std::cout << "LBUTTON DOWN" << std::endl;

			for (unsigned i = 0; i < numButtons; ++i)
			{
				if (Array[i]->MouseCheck(mousepos))
				{
					switch (Array[i]->buttype)
					{
					case(Button::BUTTON_PLAY):
						StateManager::getInstance()->ChangeState(S_PLAYING);
						break;
					case(Button::BUTTON_OPTIONS):
						SceneManager::currscene = 3;
						break;
					case(Button::BUTTON_EXIT):
						exit(0);
						break;
					}

				}
			}
		}
		else if (bLButtonState && !Application::IsMousePressed(0))
		{
			bLButtonState = false;
			std::cout << "LBUTTON UP" << std::endl;
		}
	}
}

void StateManager::ChangeState(States state)
{
	this->m_currstate = state;
}

States StateManager::GetState()
{
	return this->m_currstate;
}

void StateManager::Render()
{
	glDisable(GL_DEPTH_TEST);

	//Options
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, 0.f);
	modelStack.Scale(2.f, 2.f, 1.f);
	RenderMesh(MenuButton[PAUSE_MENU], false);
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
	

	modelStack.PushMatrix();
	modelStack.Translate(0.5f, 0.4f, 0.f);
	modelStack.Scale(0.25f, 0.25f, 1.f);
	RenderMesh(MenuButton[PAUSE_CONTINUE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.5f, 0.f, 0.f);
	modelStack.Scale(0.25f, 0.25f, 1.f);
	RenderMesh(MenuButton[PAUSE_OPTIONS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.5f, -0.4f, 0.f);
	modelStack.Scale(0.25f, 0.25f, 1.f);
	RenderMesh(MenuButton[PAUSE_EXIT], false);
	modelStack.PopMatrix();

	//On screen text
	std::ostringstream ss;
	ss.str(std::string());
	ss.precision(5);
	ss << "ESC to Pause/Unpause";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 7, 42);

	ss.str("");
	ss.precision(5);
	ss << "Q and E to change tools";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 7, 33);

	ss.str("");
	ss.precision(5);
	ss << "Spacebar to use tool";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 7, 24);

	ss.str("");
	ss.precision(5);
	ss << "Options:";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 7, 15);

	ss.str("");
	ss.precision(5);
	ss << " change mouse controls";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 7, 12);
}

void StateManager::RenderMesh(Mesh * mesh, bool enableLight)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

StateManager::StateManager()
{
	
}
