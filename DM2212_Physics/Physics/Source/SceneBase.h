#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include "SpriteAnimation.h"
#include "minimap.h"
#include <vector>

enum GEOMETRY_TYPE
{
	GEO_AXES,
	GEO_TEXT,
	GEO_BALL,
	GEO_CUBE,
	GEO_SHIP,
	GEO_GRID,

	//SP 3
	GEO_TEST_ANIMATION,
	GEO_TEXTBOX,
	GEO_ITEMSELECT,
	NUM_GEOMETRY,
};

enum MAP_TYPE
{
	GEO_MAPBG,
	GEO_MAPBORDER,
	NUM_MAP,
};

enum BG_TYPE
{
	GEO_MORNING,
	GEO_MIDDAY,
	GEO_EVENING,
	GEO_DUSK,
	GEO_BONUS,
	NUM_BG,
};

enum TOOLBOX_TYPE //Tools UI
{
	//s means select
	GEO_sPICKAXE,
	GEO_sCANNON,
	GEO_sDRILL,
	GEO_sTHUMPER,
	GEO_sDYNAMITE,
	GEO_sMISSILE,
	NUM_TOOLBOX,
};

enum EDITORBOX_TYPE //Editor tools UI
{
	//s means select
	GEO_sGLASS,
	GEO_sWOOD,
	GEO_sBRICK,
	GEO_sMETAL,
	NUM_EDITORBOX,
};

enum BLOCK_TYPE
{
	//Blocks
	GEO_GRASS = 0,
	GEO_GLASS,
	GEO_WOOD,
	GEO_METAL,
	GEO_BRICK,
	NUM_BLOCKS,
};

enum TOOL_TYPE
{
	NONE = 0,
	GEO_PICKAXE,
	GEO_CANNON,
	GEO_DRILL,
	GEO_THUMPER,
	GEO_MISSILE,
	GEO_DYNAMITE,
	NUM_TOOLS,
};

enum BUTTONS
{
	MENU_NONE = 0,
	MENU_PLAY,
	MENU_SANDBOX,
	MENU_OPTIONS,
	MENU_EXIT,
	MENU_BACKGROUND,
	ARROW,
	LOCK,
	MAX_MENU,
};

class SceneBase : public Scene
{
public:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);

	MS modelStack;
	MS viewStack;
	MS projectionStack;

protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	//Block list
	Mesh* BlockList[NUM_BLOCKS][3];
	//Tool list
	Mesh* ToolList[NUM_TOOLS];
	//Maplist
	Mesh* Maplist[NUM_MAP];
	//Background list
	Mesh* BGlist[NUM_BG];
	//Toolbox list
	Mesh* Toolboxlist[NUM_TOOLBOX];
	//Editorbox list
	Mesh* Editorboxlist[NUM_EDITORBOX];
	//MENU BUTTONS
	Mesh* Buttons[MAX_MENU];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Camera camera;

	Light lights[1];

	bool bLightEnabled;

	float fps;
};

#endif