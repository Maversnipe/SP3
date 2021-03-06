#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "SoundEngine.h"

#include <sstream>

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
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

	m_programID = LoadShaders( "Shader//comg.vertexshader", "Shader//comg.fragmentshader" );
	
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

	for(int i = 0; i < NUM_GEOMETRY; ++i)
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

	//----------Sprite animation ^-^----------
	//meshList[GEO_TEST_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("phish", 1, 6, 1.f);
	//meshList[GEO_TEST_ANIMATION]->textureID = LoadTGA("Image//maskfish.tga");
	//SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_TEST_ANIMATION]);
	////use dynamic_cast because mesh doesn't have animation
	//if (sa)
	//{
	//	sa->m_anim = new Animation();
	//	sa->m_anim->Set(0, 5, 0, 1.f, true);
	//}

	//textbox
	meshList[GEO_TEXTBOX] = MeshBuilder::GenerateQuad("ship", Color(1, 1, 1), 1.f);
	meshList[GEO_TEXTBOX]->textureID = LoadTGA("Image//textbox.tga");
	//item select
	meshList[GEO_ITEMSELECT] = MeshBuilder::GenerateQuad("items", Color(1, 1, 1), 1.f);
	meshList[GEO_ITEMSELECT]->textureID = LoadTGA("Image//toolboxbase2.tga");

	//Game (Load Block Textures Here)
	{
		BlockList[GEO_GRASS][0] = MeshBuilder::GenerateQuad("Grassblock", Color(0.f, 1.f, 0.f), 1.f);
		BlockList[GEO_GRASS][0]->textureID = LoadTGA("Image//blocks//ground//dirt.tga");

		BlockList[GEO_GLASS][0] = MeshBuilder::GenerateQuad("Glassblock1", Color(0.f, 0.f, 0.f), 1.f);
		BlockList[GEO_GLASS][0]->textureID = LoadTGA("Image//blocks//glass//glass_fullhealth.tga");
		BlockList[GEO_GLASS][1] = MeshBuilder::GenerateQuad("Glassblock2", Color(0.f, 0.f, 0.f), 1.f);
		BlockList[GEO_GLASS][1]->textureID = LoadTGA("Image//blocks//glass//glass_damaged.tga");

		BlockList[GEO_WOOD][0] = MeshBuilder::GenerateQuad("Woodblock1", Color(0.9f, 0.9f, 0.9f), 1.f);
		BlockList[GEO_WOOD][0]->textureID = LoadTGA("Image//blocks//wood//wood_fullhealth.tga");
		BlockList[GEO_WOOD][1] = MeshBuilder::GenerateQuad("Woodblock2", Color(0.9f, 0.9f, 0.9f), 1.f);
		BlockList[GEO_WOOD][1]->textureID = LoadTGA("Image//blocks//wood//wood_damaged.tga");
		BlockList[GEO_WOOD][2] = MeshBuilder::GenerateQuad("Woodblock3", Color(0.9f, 0.9f, 0.9f), 1.f);
		BlockList[GEO_WOOD][2]->textureID = LoadTGA("Image//blocks//wood//wood_broken.tga");

		BlockList[GEO_METAL][0] = MeshBuilder::GenerateQuad("Metalblock", Color(1.f, 1.f, 1.f), 1.f);
		BlockList[GEO_METAL][0]->textureID = LoadTGA("Image//blocks//metal//metal.tga");

		BlockList[GEO_BRICK][0] = MeshBuilder::GenerateQuad("Brickblock1", Color(1.f, 0.f, 0.f), 1.f);
		BlockList[GEO_BRICK][0]->textureID = LoadTGA("Image//blocks//brick//brick_fullhealth.tga");
		BlockList[GEO_BRICK][1] = MeshBuilder::GenerateQuad("Brickblock2", Color(1.f, 0.f, 0.f), 1.f);
		BlockList[GEO_BRICK][1]->textureID = LoadTGA("Image//blocks//brick//brick_damaged.tga");
		BlockList[GEO_BRICK][2] = MeshBuilder::GenerateQuad("Brickblock3", Color(1.f, 0.f, 0.f), 1.f);
		BlockList[GEO_BRICK][2]->textureID = LoadTGA("Image//blocks//brick//brick_broken.tga");
	}

	//tools
	{
		ToolList[GEO_PICKAXE] = MeshBuilder::GenerateQuad("Pickaxe", Color(1, 1, 1), 1);
		ToolList[GEO_PICKAXE]->textureID = LoadTGA("Image//Tools//pickaxe.tga");
		ToolList[GEO_CANNON] = MeshBuilder::GenerateQuad("Cannon", Color(1, 1, 1), 1);
		ToolList[GEO_CANNON]->textureID = LoadTGA("Image//Tools//cannon.tga");
		ToolList[GEO_DRILL] = MeshBuilder::GenerateQuad("Drill", Color(1, 1, 1), 1);
		ToolList[GEO_DRILL]->textureID = LoadTGA("Image//Tools//drill.tga");
		ToolList[GEO_THUMPER] = MeshBuilder::GenerateQuad("Thumper", Color(1, 1, 1), 1);
		ToolList[GEO_THUMPER]->textureID = LoadTGA("Image//Tools//thumper.tga");
		ToolList[GEO_MISSILE] = MeshBuilder::GenerateQuad("Missile", Color(1, 1, 1), 1);
		ToolList[GEO_MISSILE]->textureID = LoadTGA("Image//Tools//napalm.tga");
		ToolList[GEO_DYNAMITE] = MeshBuilder::GenerateQuad("dynamite", Color(1, 1, 1), 1);
		ToolList[GEO_DYNAMITE]->textureID = LoadTGA("Image//Tools//dynamite.tga");
	}

	// Projectile
	{
		Projectile[GEO_CANNONBALL] = MeshBuilder::GenerateQuad("cannonball", Color(1, 1, 1), 1);
		Projectile[GEO_CANNONBALL]->textureID = LoadTGA("Image//Tools//cannonball.tga");
		Projectile[GEO_ROCKET] = MeshBuilder::GenerateQuad("rocket", Color(1, 1, 1), 1);
		Projectile[GEO_ROCKET]->textureID = LoadTGA("Image//Tools//napalm.tga");
		
		Projectile[GEO_EXPLOSION] = MeshBuilder::GenerateSpriteAnimation("explosion", 1, 10, 1.f);
		Projectile[GEO_EXPLOSION]->textureID = LoadTGA("Image//weirdexplosion.tga");
		SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(Projectile[GEO_EXPLOSION]);
		//use dynamic_cast because mesh doesn't have animation
		if (sa)
		{
			sa->m_anim = new Animation();
			sa->m_anim->Set(0, 9, 0, 1.f, true);
		}
	}

	//minimap
	Maplist[GEO_MAPBG] = MeshBuilder::GenerateQuad("mapbg", Color(1.f, 0.f, 0.f), 1.f);
	Maplist[GEO_MAPBG]->textureID = LoadTGA("Image//Background//dusk.tga");
	Maplist[GEO_MAPBORDER] = MeshBuilder::GenerateQuad("mapborder", Color(1.f, 1.f, 1.f), 1.f);
	CMinimap::GetInstance()->SetBorder(Maplist[GEO_MAPBORDER]);

	//Backgrounds
	BGlist[GEO_MORNING] = MeshBuilder::GenerateQuad("morningBG", Color(1.f, 0.f, 0.f), 1.f);
	BGlist[GEO_MORNING]->textureID = LoadTGA("Image//Background//morning.tga");
	BGlist[GEO_MIDDAY] = MeshBuilder::GenerateQuad("middayBG", Color(1.f, 0.f, 0.f), 1.f);
	BGlist[GEO_MIDDAY]->textureID = LoadTGA("Image//Background//midday.tga");
	BGlist[GEO_EVENING] = MeshBuilder::GenerateQuad("eveningBG", Color(1.f, 0.f, 0.f), 1.f);
	BGlist[GEO_EVENING]->textureID = LoadTGA("Image//Background//evening.tga");
	BGlist[GEO_DUSK] = MeshBuilder::GenerateQuad("duskBG", Color(1.f, 0.f, 0.f), 1.f);
	BGlist[GEO_DUSK]->textureID = LoadTGA("Image//Background//dusk.tga");
	BGlist[GEO_BONUS] = MeshBuilder::GenerateQuad("bonusBG", Color(1.f, 0.f, 0.f), 1.f);
	BGlist[GEO_BONUS]->textureID = LoadTGA("Image//Background//bonus.tga");

	//Main Menu
	Buttons[MENU_BACKGROUND] = MeshBuilder::GenerateQuad("backround", Color(1.f, 0.f, 0.f), 1.f);
	Buttons[MENU_BACKGROUND]->textureID = LoadTGA("Image//mainmenu//mainmenu.tga");
	Buttons[MENU_PLAY] = MeshBuilder::GenerateQuad("play", Color(1.f, 0.f, 0.f), 1.f);
	Buttons[MENU_PLAY]->textureID = LoadTGA("Image//mainmenu//play.tga");
	Buttons[MENU_OPTIONS] = MeshBuilder::GenerateQuad("options", Color(1.f, 0.f, 0.f), 1.f);
	Buttons[MENU_OPTIONS]->textureID = LoadTGA("Image//mainmenu//options.tga");
	Buttons[MENU_EXIT] = MeshBuilder::GenerateQuad("exit", Color(1.f, 0.f, 0.f), 1.f);
	Buttons[MENU_EXIT]->textureID = LoadTGA("Image//mainmenu//quit.tga");
	Buttons[MENU_SANDBOX] = MeshBuilder::GenerateQuad("sandbox", Color(1.f, 0.f, 0.f), 1.f);
	Buttons[MENU_SANDBOX]->textureID = LoadTGA("Image//mainmenu//editor.tga");
	Buttons[ARROW] = MeshBuilder::GenerateQuad("arrow", Color(1.f, 0.f, 0.f), 1.f);
	Buttons[ARROW]->textureID = LoadTGA("Image//arrow.tga");
	Buttons[LOCK] = MeshBuilder::GenerateQuad("lock", Color(1.f, 0.f, 0.f), 1.f);
	Buttons[LOCK]->textureID = LoadTGA("Image//lock.tga");

	Gameover[END_MENU] = MeshBuilder::GenerateQuad("background", Color(1.f, 0.f, 0.f), 1.f);
	Gameover[END_MENU]->textureID = LoadTGA("Image//mainmenu//gameover.tga");
	Gameover[END_BUTTON] = MeshBuilder::GenerateQuad("Back", Color(1.f, 0.f, 0.f), 1.f);
	Gameover[END_BUTTON]->textureID = LoadTGA("Image//mainmenu//play.tga");
	
	//UI (toolbox)
	Toolboxlist[GEO_sPICKAXE] = MeshBuilder::GenerateQuad("selectpickaxe", Color(1.f, 0.f, 0.f), 1.f);
	Toolboxlist[GEO_sPICKAXE]->textureID = LoadTGA("Image//toolbox//selectpickaxe.tga");
	Toolboxlist[GEO_sCANNON] = MeshBuilder::GenerateQuad("selectcannon", Color(1.f, 0.f, 0.f), 1.f);
	Toolboxlist[GEO_sCANNON]->textureID = LoadTGA("Image//toolbox//selectcannon.tga");
	Toolboxlist[GEO_sDRILL] = MeshBuilder::GenerateQuad("selectdrill", Color(1.f, 0.f, 0.f), 1.f);
	Toolboxlist[GEO_sDRILL]->textureID = LoadTGA("Image//toolbox//selectdrill.tga");
	Toolboxlist[GEO_sTHUMPER] = MeshBuilder::GenerateQuad("selectthumper", Color(1.f, 0.f, 0.f), 1.f);
	Toolboxlist[GEO_sTHUMPER]->textureID = LoadTGA("Image//toolbox//selectthumper.tga");
	Toolboxlist[GEO_sDYNAMITE] = MeshBuilder::GenerateQuad("selectdynamite", Color(1.f, 0.f, 0.f), 1.f);
	Toolboxlist[GEO_sDYNAMITE]->textureID = LoadTGA("Image//toolbox//selectdynamite.tga");
	Toolboxlist[GEO_sMISSILE] = MeshBuilder::GenerateQuad("selectmissile", Color(1.f, 0.f, 0.f), 1.f);
	Toolboxlist[GEO_sMISSILE]->textureID = LoadTGA("Image//toolbox//selectmissile.tga");

	//UI (editor)
	Editorboxlist[GEO_sGLASS] = MeshBuilder::GenerateQuad("sglass", Color(1.f, 0.f, 0.f), 1.f);
	Editorboxlist[GEO_sGLASS]->textureID = LoadTGA("Image//editorbox//glassedit.tga");
	Editorboxlist[GEO_sWOOD] = MeshBuilder::GenerateQuad("swood", Color(1.f, 0.f, 0.f), 1.f);
	Editorboxlist[GEO_sWOOD]->textureID = LoadTGA("Image//editorbox//woodedit.tga");
	Editorboxlist[GEO_sBRICK] = MeshBuilder::GenerateQuad("sbrick", Color(1.f, 0.f, 0.f), 1.f);	
	Editorboxlist[GEO_sBRICK]->textureID = LoadTGA("Image//editorbox//brickedit.tga");
	Editorboxlist[GEO_sMETAL] = MeshBuilder::GenerateQuad("smetal", Color(1.f, 0.f, 0.f), 1.f);
	Editorboxlist[GEO_sMETAL]->textureID = LoadTGA("Image//editorbox//metaledit.tga");

	//Load sound (test)
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("test", "Sound//Theme.mp3");

	bLightEnabled = false;
}

void SceneBase::Update(double dt)
{
	//Keyboard Section
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//----------Sprite animation ^-^----------
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>
		(Projectile[GEO_EXPLOSION]);
	//use dynamic_cast because mesh doesn't have animation
	if (sa)
	{
		sa->Update(dt);
		sa->m_anim->animActive = true;
	}

	fps = (float)(1.f / dt);
	//CSoundEngine::GetInstance()->PlayASound("test"); //test sound
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
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
	if(mesh->textureID > 0)
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
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneBase::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}

	//Clean Blocks
	/*for (int i = 0; i < NUM_BLOCKS; ++i)
	{
		if (BlockList[i])
			delete BlockList[i];
	}*/

	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
