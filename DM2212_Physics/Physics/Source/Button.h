#pragma once
#include "Vector3.h"

class Button
{
public:
	enum BUTTON_TYPE
	{
		BUTTON_NONE,
		BUTTON_PLAY,
		BUTTON_SANDBOX,
		BUTTON_OPTIONS,
		BUTTON_EXIT,
		EDITOR_BACKGROUND_LEFT,
		EDITOR_BACKGROUND_RIGHT,
		EDITOR_MONEY_LEFT,
		EDITOR_MONEY_RIGHT,
		EDITOR_PICKAXE,
		EDITOR_CANNON,
		EDITOR_THUMPER,
		EDITOR_DRILL,
		EDITOR_MISSILE,
		EDITOR_DYNAMITE,
		BUTTON_MAX,//max

	};
	Button();
	Button(Vector3 scale, Vector3 pos, BUTTON_TYPE buttype);
	~Button();
	
	Vector3 GetScale();
	Vector3 GetPos();
	void SetScale(Vector3 scale);
	void SetPos(Vector3 pos);

	bool MouseCheck(Vector3 mousepos);

	void SetBig(bool big);
	bool GetBig();

	BUTTON_TYPE buttype;

private:
	Vector3 scale;
	Vector3 pos;
	bool big;
	bool active;
};