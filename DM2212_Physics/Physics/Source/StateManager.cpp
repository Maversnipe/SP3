#include "StateManager.h"
#include "Application.h"

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
	glDepthFunc(GL_LESS);

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
	MenuButton[MENU_BACKGROUND] = MeshBuilder::GenerateQuad("backround", Color(1.f, 0.f, 0.f), 1.f);
	MenuButton[MENU_BACKGROUND]->textureID = LoadTGA("Image//mainmenu//mainmenu.tga");
	MenuButton[MENU_PLAY] = MeshBuilder::GenerateQuad("play", Color(1.f, 0.f, 0.f), 1.f);
	MenuButton[MENU_PLAY]->textureID = LoadTGA("Image//mainmenu//play.tga");
	MenuButton[MENU_OPTIONS] = MeshBuilder::GenerateQuad("options", Color(1.f, 0.f, 0.f), 1.f);
	MenuButton[MENU_OPTIONS]->textureID = LoadTGA("Image//mainmenu//options.tga");
	MenuButton[MENU_EXIT] = MeshBuilder::GenerateQuad("exit", Color(1.f, 0.f, 0.f), 1.f);
	MenuButton[MENU_EXIT]->textureID = LoadTGA("Image//mainmenu//quit.tga");
	MenuButton[MENU_SANDBOX] = MeshBuilder::GenerateQuad("sandbox", Color(1.f, 0.f, 0.f), 1.f);
	MenuButton[MENU_SANDBOX]->textureID = LoadTGA("Image//mainmenu//editor.tga");

	//Set limits
	m_fworldheight = 100.f;
	m_fworldwidth = m_fworldheight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	bLightEnabled = false;
}

void StateManager::Update(double dt)
{
	if (Application::IsKeyPressed(VK_ESCAPE))
	{
		StateManager::getInstance()->ChangeState(S_PAUSED);
	}
	else if (Application::IsKeyPressed(VK_F1))
	{
		StateManager::getInstance()->ChangeState(S_PLAYING);
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

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
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
	m_fworldheight = 100.f;
	m_fworldwidth = m_fworldheight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	glDisable(GL_DEPTH_TEST);

	//Options
	modelStack.PushMatrix();
	modelStack.Translate(0.f, 0.f, -1.f);
	modelStack.Scale(2.f, 2.f, 1.f);
	RenderMesh(MenuButton[MENU_BACKGROUND], false);
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);

	modelStack.PushMatrix();
	modelStack.Translate(0.5f, 0.4f, 0.f);
	modelStack.Scale(0.25f, 0.25f, 1.f);
	RenderMesh(MenuButton[MENU_PLAY], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.5f, 0.f, 0.f);
	modelStack.Scale(0.25f, 0.25f, 1.f);
	RenderMesh(MenuButton[MENU_OPTIONS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.5f, -0.4f, 0.f);
	modelStack.Scale(0.25f, 0.25f, 1.f);
	RenderMesh(MenuButton[MENU_EXIT], false);
	modelStack.PopMatrix();
}

void StateManager::RenderMesh(Mesh * mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

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
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

StateManager::StateManager()
{
	
}
